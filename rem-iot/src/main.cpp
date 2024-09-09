#include "stdlib.h"

#include "Adafruit_BME280.h"
#include "EEPROM.h"
#include "PM1006K.h"
#include "PubSubClient.h"
#include "WiFi.h"
#include "Wire.h"

#include "controller.hpp"
#include "mqtt_msg.hpp"
#include "pin_config.hpp"
#include "settings_manager.hpp"
#include "wifi_controller.hpp"
#include "terminal.hpp"
#include "tasks.hpp"

#define INIT_DEBUG true

#define PUBLISH_DATA_STACK 2048
#define PUBLISH_STATUS_STACK 4096
#define PUBLISH_TERMINAL_STACK 4096

#define SAMPLE_RATE 5000

// Statically create pointers to all of the providers.
// These instances should last the lifetime of the program
REMController *controller;
SettingsManager *settingsManager;
PM1006K *pm1006k;
Adafruit_BME280 *bme280;
WiFiClient espClient;
Terminal *terminal;
PubSubClient * pubSubClient;
REMTaskProviders *providers;

// Create a queue to handle the messages that are sent to the MQTT server
static QueueHandle_t msgQueue = xQueueCreate(10, sizeof(MQTTMsg *));

static BaseType_t prvRebootCommand(char *pcWriteBuffer, size_t xWriteBufferLen,
                                   const char *pcCommandString);

static const CLI_Command_Definition_t xRebootCommand = {
    "reboot", "\r\nreboot:\r\n this command reboots the esp32\r\n",
    prvRebootCommand, 0};

static BaseType_t prvUpdateSettingCommand(char *pcWriteBuffer,
                                       size_t xWriteBufferLen,
                                       const char *pcCommandString);

static const CLI_Command_Definition_t xUpdateSettingCommand = {
    "update-setting",
    "\r\nupdate-setting <NAME> <VALUE>:\r\n This command reboots the esp32. Valid settings include `ssid` and `password`\r\n",
    prvUpdateSettingCommand, 2};

static BaseType_t prvPrintSettingsCommand(char *pcWriteBuffer,
                                       size_t xWriteBufferLen,
                                       const char *pcCommandString);

static const CLI_Command_Definition_t xPrintSettingsCommand = {
    "print-settings",
    "\r\nprint-settings: \r\n This command will return a list of all of the settings and their values. \r\n",
    prvPrintSettingsCommand, 0};

static BaseType_t prvDebugCommand(char *pcWriteBuffer,
                                       size_t xWriteBufferLen,
                                       const char *pcCommandString);

static const CLI_Command_Definition_t xDebugCommand = {
    "debug",
    "\r\ndebug: \r\n This toggles the debug logging\r\n",
    prvDebugCommand, 0 };

static BaseType_t prvWiFiStatusCommand(char *pcWriteBuffer,
                                       size_t xWriteBufferLen,
                                       const char *pcCommandString);

static const CLI_Command_Definition_t xWiFIStatusCommand = {
    "wifi-status",
    "\r\nwifi-status: \r\n Returns the wifi connected status\r\n",
    prvWiFiStatusCommand, 0 };

/**
 * Setup the wifi connection and the NTP server. If the clock is not synced
 * with the NTP server, the function will return false
 */
void setupWiFi() {
  WiFiController *wifiController = new WiFiController(&WiFi, terminal, settingsManager);
  if (!wifiController->setupWiFi()) {
    terminal->debugln("Wifi Setup Failed"); 
  }

  wifiController->setupSNTP();
  if (!wifiController->verifyClockSync()) {
    terminal->debugln("Failed to sync the clock with the NTP server");
  }

  // Controller isn't needed again execpt for initial networking config
  delete wifiController;
}

/**
 * Setup the terminal and the terminal task that will handle the input and output
 */
void setupTerminal() {

  Serial.begin(115200);
  terminal = new Terminal(INIT_DEBUG, &Serial);
  
  // Setup the terminal task that handles the input and output of the terminal
  xTaskCreate(TerminalTask, "Terminal Task", PUBLISH_TERMINAL_STACK, terminal, 1,
            NULL);

  // Setup CLI Commands
  FreeRTOS_CLIRegisterCommand(&xRebootCommand);
  FreeRTOS_CLIRegisterCommand(&xUpdateSettingCommand);
  FreeRTOS_CLIRegisterCommand(&xPrintSettingsCommand);
  FreeRTOS_CLIRegisterCommand(&xDebugCommand);
  FreeRTOS_CLIRegisterCommand(&xWiFIStatusCommand);
}

/**
 * Initializes the pubsub client which internally creates a MQTT connection to the
 * server. The server connected too is determeind by the mqtt server setting.
 */
void setupPubSubClient() {
  pubSubClient = new PubSubClient(espClient);
  pubSubClient->setServer(settingsManager->getSetting(MQTT_SERVER_ID), 1883);
  pubSubClient->connect("arduinoClient");

  while(!pubSubClient->connected()) {
    terminal->debugln("Failed to connect to MQTT server, retrying...");
    delay(1000);
  }

}

void setup() {
  // Turn on Fan
  pinMode(FAN, OUTPUT);
  digitalWrite(FAN, HIGH);

  setupTerminal();

  // Setup EEPROM
  int count = 0;
  while (count < 5 && !EEPROM.begin(EEPROM_SIZE)) {
    terminal->debugln("Failed to setup EEPROM, retrying...");
    count++;
    delay(1000);
  }

  // Setup settings manager and load settings from eeprom
  settingsManager = new SettingsManager(terminal, &EEPROM);
  if (!settingsManager->loadSettings()) {
    terminal->debugln("Failed to load settings");
  }

  // Setup network utilities including the network connection, 
  // sntp, and the mqtt client
  setupWiFi();
  setupPubSubClient();

  // Setup Controller and Controller Depedencies
  Serial1.begin(PM1006K::BAUD_RATE, SERIAL_8N1, PM1006K_RX_PIN,
  PM1006K_TX_PIN); 
  pm1006k = new PM1006K(&Serial1);

  // Setup I2C and BME280 Driver
  Wire.begin(I2C_SDA, I2C_SCL);
  bme280 = new Adafruit_BME280();
  bme280->begin(BME280_ADDRESS, &Wire);

  // Setup Controller
  controller = new REMController(pm1006k, bme280, terminal, settingsManager, &msgQueue);

  // Setup the remaining tasks to queue data and the status of the device
  xTaskCreate(QueueDataTask, "Queue Data", PUBLISH_DATA_STACK, controller, 1,
  NULL); 

  xTaskCreate(QueueStatusTask, "Status Data", PUBLISH_DATA_STACK, controller, 1,
  NULL); 

  providers = new REMTaskProviders(controller, settingsManager, terminal, pubSubClient, &msgQueue);
  xTaskCreate(PublishMQTTMsg, "Publish Status",
  PUBLISH_STATUS_STACK, providers, 1, NULL);  

  terminal->debugln("Setup Room Environment Monitor");
}

// Main loop is uneeded because we are using freertos tasks to persist the
// application
void loop() { return; }

//
// Begin the CLI Command Definitions
//

BaseType_t prvRebootCommand(char *pcWriteBuffer, size_t xWriteBufferLen,
                            const char *pcCommandString) {
  ESP.restart();

  // Never reached because the esp32 reboots fully
  return pdTRUE;
}

BaseType_t prvUpdateSettingCommand(char *pcWriteBuffer, size_t xWriteBufferLen,
                                const char *pcCommandString) {

  BaseType_t nameParamToStringLength, valueParamToStringLength; 
  const char * nameParam = FreeRTOS_CLIGetParameter( pcCommandString,
                                            1,
                                            &nameParamToStringLength );

  const char * valueParam = FreeRTOS_CLIGetParameter( pcCommandString,
                                            2,
                                            &valueParamToStringLength );

  // Call the update function and validate the response note that if it is false, it coul;d
  if (settingsManager->updateSetting(nameParam, valueParam, (int)valueParamToStringLength)) {
    snprintf(pcWriteBuffer, xWriteBufferLen, "Update setting successfully");
  } else {
    snprintf(pcWriteBuffer, xWriteBufferLen, "Failed to update setting check to make sure the setting name is valid and the length of the value is under %d characters", SETTING_LEN);
  }

  return pdFALSE;
}

BaseType_t prvDebugCommand(char *pcWriteBuffer, size_t xWriteBufferLen,
                                const char *pcCommandString) {

  bool debug_val = terminal->toggleDebug();
  snprintf(pcWriteBuffer, xWriteBufferLen, "Debug logging is now is now %s\r\n", debug_val ? "enabled" : "disabled");

  return pdFALSE;
}

BaseType_t prvPrintSettingsCommand(char *pcWriteBuffer, size_t xWriteBufferLen,
                                const char *pcCommandString) {

  BaseType_t nameParamToStringLength, valueParamToStringLength; 

  // If we get a false response from the print settings function, print an error message
  // It mostlikely is a memory allocation issue where xWriteBufferLen is too small
  if (!settingsManager->printSettings(pcWriteBuffer, xWriteBufferLen)) {
    snprintf(pcWriteBuffer, xWriteBufferLen, "Failed to print the settings\r\n");
  }

  return pdFALSE;
}

BaseType_t prvWiFiStatusCommand(char *pcWriteBuffer, size_t xWriteBufferLen,
                                const char *pcCommandString) {

  wl_status_t status = WiFi.status();
  snprintf(pcWriteBuffer, xWriteBufferLen, "WiFi status: %d \r\n", status);

  return pdFALSE;
}
