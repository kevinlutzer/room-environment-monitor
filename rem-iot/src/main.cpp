#include "stdlib.h"

#include "Adafruit_BME280.h"
#include "Adafruit_SGP40.h"
#include "Adafruit_NeoPixel.h"
#include "Adafruit_SGP40.h"
#include "EEPROM.h"
#include "PM1006K.h"
#include "PubSubClient.h"
#include "UUID.h"
#include "WiFi.h"
#include "Wire.h"

#include "mqtt_msg.hpp"
#include "pin_config.hpp"
#include "sensor_adapter.hpp"
#include "settings_manager.hpp"
#include "tasks.hpp"
#include "terminal.hpp"
#include "wifi_controller.hpp"

#define INIT_DEBUG true

#define PUBLISH_DATA_STACK 2048
#define PUBLISH_STATUS_STACK 4096
#define PUBLISH_TERMINAL_STACK 4096

#define SAMPLE_RATE 5000

// Statically create pointers to all of the providers.
// These instances should last the lifetime of the program
SensorAdapter *sensorAdapter;
SettingsManager *settingsManager;
PM1006K *pm1006k;
Adafruit_BME280 *bme280;
Adafruit_SGP40 *sgp40;
Adafruit_NeoPixel *neoPixel;
WiFiClient espClient;
Terminal *terminal;
PubSubClient *pubSubClient;
REMTaskProviders *providers;
LEDController *ledController;
UUID *uuidGenerator;

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
    "\r\nupdate-setting <NAME> <VALUE>:\r\n This command reboots the esp32. "
    "Valid settings include `ssid` and `password`\r\n",
    prvUpdateSettingCommand, 2};

static BaseType_t prvPrintSettingsCommand(char *pcWriteBuffer,
                                          size_t xWriteBufferLen,
                                          const char *pcCommandString);

static const CLI_Command_Definition_t xPrintSettingsCommand = {
    "print-settings",
    "\r\nprint-settings: \r\n This command will return a list of all of the "
    "settings and their values. \r\n",
    prvPrintSettingsCommand, 0};

static BaseType_t prvDebugCommand(char *pcWriteBuffer, size_t xWriteBufferLen,
                                  const char *pcCommandString);

static const CLI_Command_Definition_t xDebugCommand = {
    "debug", "\r\ndebug: \r\n This toggles the debug logging\r\n",
    prvDebugCommand, 0};

static BaseType_t prvWiFiStatusCommand(char *pcWriteBuffer,
                                       size_t xWriteBufferLen,
                                       const char *pcCommandString);

static const CLI_Command_Definition_t xWiFIStatusCommand = {
    "wifi-status",
    "\r\nwifi-status: \r\n Returns the wifi connected status\r\n",
    prvWiFiStatusCommand, 0};

static BaseType_t prvPrintSensorDataCommand(char *pcWriteBuffer,
                                            size_t xWriteBufferLen,
                                            const char *pcCommandString);

static const CLI_Command_Definition_t xPrintSensorDataCommand = {
    "print-sensor-data",
    "\r\nprint-sensor-data: \r\n Prints a snapshot of the sensor data.\r\n",
    prvPrintSensorDataCommand, 0};

/**
 * Setup the wifi connection and the NTP server. If the clock is not synced
 * with the NTP server, the function will return false
 */
void setupWiFi() {
  WiFiController *wifiController =
      new WiFiController(&WiFi, terminal, settingsManager);
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
 * Setup the terminal and the terminal task that will handle the input and
 * output
 */
void setupTerminal() {

  Serial.begin(115200);
  terminal = new Terminal(INIT_DEBUG, &Serial);

  // Setup the terminal task that handles the input and output of the terminal
  xTaskCreate(TerminalTask, "Terminal Task", PUBLISH_TERMINAL_STACK, terminal,
              1, NULL);

  // Setup CLI Commands that can be run as soon as the terminal task is started
  FreeRTOS_CLIRegisterCommand(&xRebootCommand);
  FreeRTOS_CLIRegisterCommand(&xDebugCommand);
}

/**
 * Initializes the pubsub client which internally creates a MQTT connection to
 * the server. The server connected too is determeind by the mqtt server
 * setting.
 */
void setupPubSubClient() {
  pubSubClient = new PubSubClient(espClient);
  pubSubClient->setServer(settingsManager->getSetting(MQTT_SERVER_ID), 1883);
  pubSubClient->connect("arduinoClient");

  while (!pubSubClient->connected()) {
    terminal->debugln("Failed to connect to MQTT server, retrying...");
    delay(1000);
  }
}

void setup() {

  Adafruit_NeoPixel NeoPixel(3, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
  NeoPixel.clear(); // set all pixel colors to 'off'. It only takes effect if
                    // pixels.show() is called

  // turn pixels to green one-by-one with delay between each pixel
  for (int pixel = 0; pixel < 3; pixel++) { // for each pixel
    NeoPixel.setPixelColor(
        pixel,
        NeoPixel.Color(0, 255,
                       0)); // it only takes effect if pixels.show() is called
    NeoPixel.show();        // update to the NeoPixel Led Strip

    delay(500); // 500ms pause between each pixel
  }

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

  // Add tasks related to settings since settings manager is instantiated
  FreeRTOS_CLIRegisterCommand(&xUpdateSettingCommand);
  FreeRTOS_CLIRegisterCommand(&xPrintSettingsCommand);

  // Setup network utilities including the network connection,
  // sntp, and the mqtt client
  setupWiFi();
  setupPubSubClient();

  // Setup Controller and Controller Depedencies
  Serial1.begin(PM1006K::BAUD_RATE, SERIAL_8N1, PM1006K_RX_PIN, PM1006K_TX_PIN);
  pm1006k = new PM1006K(&Serial1);

  // Setup I2C and BME280 Driver
  Wire.begin(I2C_SDA, I2C_SCL);
  bme280 = new Adafruit_BME280();
  if (!bme280->begin(BME280_ADDRESS, &Wire)) {
    terminal->debugln("Failed to setup BME280");
  }

  // Setup the SGP40 Driver
  sgp40 = new Adafruit_SGP40();
  if (!sgp40->begin(&Wire)) {
    terminal->debugln("Failed to setup SGP40");
  }

  // Setup SGP40 Driver
  sgp40 = new Adafruit_SGP40();
  sgp40->begin(&Wire);
  
  raw = sgp40.measureRaw();

  // Setup the neopixel controller and clear the pixels
  neoPixel =
      new Adafruit_NeoPixel(NUM_NEOPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
  neoPixel->begin();
  neoPixel->clear();
  ledController = new LEDController(neoPixel, terminal);

  // Setup Controller
  sensorAdapter = new SensorAdapter(pm1006k, bme280, sgp40, terminal);

  // Create UUID Generator used for generating unique ids
  randomSeed(analogRead(A0) | analogRead(A1) | analogRead(A2));
  uint32_t rn = random();

  uuidGenerator = new UUID();
  uuidGenerator->seed(rn);
  uuidGenerator->setRandomMode(); // This is important for the UUID to be unique
                                  // between reboots

  // Remainder of the commands that depend on providers instantiated above
  FreeRTOS_CLIRegisterCommand(&xWiFIStatusCommand);
  FreeRTOS_CLIRegisterCommand(&xPrintSensorDataCommand);

  // Setup the task providers and the task that publishes the messages
  providers = new REMTaskProviders(sensorAdapter, settingsManager, terminal,
                                   pubSubClient, uuidGenerator, ledController);

  xTaskCreate(PublishMQTTMsg, "Publish MQTT", PUBLISH_STATUS_STACK, providers,
              1, NULL);

  xTaskCreate(QueueDataTask, "Queue Data", PUBLISH_DATA_STACK, providers, 1,
              NULL);

  xTaskCreate(QueueStatusTask, "Status Data", PUBLISH_DATA_STACK, providers, 1,
              NULL);

  xTaskCreate(LEDUpdateTask, "LED Update Task", PUBLISH_DATA_STACK, providers,
              1, NULL);

  terminal->debugln("Started tasks...");
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
  const char *nameParam =
      FreeRTOS_CLIGetParameter(pcCommandString, 1, &nameParamToStringLength);

  const char *valueParam =
      FreeRTOS_CLIGetParameter(pcCommandString, 2, &valueParamToStringLength);

  // Call the update function and validate the response note that if it is
  // false, it coul;d
  if (settingsManager->updateSetting(nameParam, valueParam,
                                     (int)valueParamToStringLength)) {
    snprintf(pcWriteBuffer, xWriteBufferLen, "Update setting successfully");
  } else {
    snprintf(pcWriteBuffer, xWriteBufferLen,
             "Failed to update setting check to make sure the setting name is "
             "valid and the length of the value is under %d characters",
             SETTING_LEN);
  }

  return pdFALSE;
}

BaseType_t prvDebugCommand(char *pcWriteBuffer, size_t xWriteBufferLen,
                           const char *pcCommandString) {

  bool debug_val = terminal->toggleDebug();
  snprintf(pcWriteBuffer, xWriteBufferLen, "Debug logging is now %s\r\n",
           debug_val ? "enabled" : "disabled");

  return pdFALSE;
}

BaseType_t prvPrintSettingsCommand(char *pcWriteBuffer, size_t xWriteBufferLen,
                                   const char *pcCommandString) {

  BaseType_t nameParamToStringLength, valueParamToStringLength;

  // If we get a false response from the print settings function, print an error
  // message It mostlikely is a memory allocation issue where xWriteBufferLen is
  // too small
  if (!settingsManager->printSettings(pcWriteBuffer, xWriteBufferLen)) {
    snprintf(pcWriteBuffer, xWriteBufferLen,
             "Failed to print the settings\r\n");
  }

  return pdFALSE;
}

BaseType_t prvWiFiStatusCommand(char *pcWriteBuffer, size_t xWriteBufferLen,
                                const char *pcCommandString) {

  // Grab the wifi information including the ip
  wl_status_t status = WiFi.status();
  String ip = WiFi.localIP().toString();

  if (!ip.length()) {
    snprintf(pcWriteBuffer, xWriteBufferLen, "WiFi status: %d \r\nIP: N/A \r\n",
             status);
  } else {
    const char *ipCStr = ip.c_str();
    snprintf(pcWriteBuffer, xWriteBufferLen, "WiFi status: %d \r\nIP: %s\r\n",
             status, ipCStr);
  }

  return pdFALSE;
}

BaseType_t prvPrintSensorDataCommand(char *pcWriteBuffer,
                                     size_t xWriteBufferLen,
                                     const char *pcCommandString) {

  // If we have a mismatched config for the output buffer length this will
  // return false
  if (!sensorAdapter->printData(pcWriteBuffer, xWriteBufferLen)) {
    snprintf(pcWriteBuffer, xWriteBufferLen,
             "Failed to print the settings, write buffer passed is not long "
             "enough.\r\n");
  }

  return pdFALSE;
}
