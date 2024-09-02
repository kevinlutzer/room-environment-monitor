#include "stdlib.h"

#include "Adafruit_BME280.h"
#include "EEPROM.h"
#include "PM1006K.h"
#include "PubSubClient.h"
#include "WiFi.h"
#include "Wire.h"

#include "controller.hpp"
#include "pin_config.hpp"
#include "settings_manager.hpp"
#include "terminal.hpp"

#define DEBUG true

#define PUBLISH_DATA_STACK 2048
#define PUBLISH_STATUS_STACK 1024
#define PUBLISH_TERMINAL_STACK 1024

// Statically create pointers to all of the providers.
// These instances should last the lifetime of the program
REMController *controller;
SettingsManager *settingsManager;
PM1006K *pm1006k;
Adafruit_BME280 *bme280;
PubSubClient *pubsubClient;
WiFiClient espClient;
Terminal *terminal;

int count = 0;

// Task Defs
void PublishDataTask(void *paramater);
void PublishStatusTask(void *paramater);
void TerminalTask(void *paramater);

static BaseType_t prvRebootCommand(char *pcWriteBuffer, size_t xWriteBufferLen,
                                   const char *pcCommandString);

static const CLI_Command_Definition_t xRebootCommand = {
    "reboot", "\r\nreboot:\r\n this command reboots the esp32\r\n\r\n",
    prvRebootCommand, 0};

static BaseType_t prvReadEEPROMCommand(char *pcWriteBuffer,
                                       size_t xWriteBufferLen,
                                       const char *pcCommandString);

static const CLI_Command_Definition_t xReadEEPROMCommand = {
    "read-eeprom",
    "\r\nread-eeprom:\r\n this command reboots the esp32\r\n\r\n",
    prvReadEEPROMCommand, 1};
void setup() {
  // Turn on Fan
  pinMode(FAN, OUTPUT);
  digitalWrite(FAN, HIGH);

  // Setup debug construct
  Serial.begin(115200);
  terminal = new Terminal(DEBUG, &Serial);

  // Setup EEPROM
  int count = 0;
  while (count < 5 && !EEPROM.begin(EEPROM_SIZE)) {
    terminal->debugln("Failed to setup EEPROM, retrying...");
    count++;
    delay(1000);
  }

  // Setup secrets instance
  settingsManager = new SettingsManager(terminal, &EEPROM);

  if (!settingsManager->loadSettings()) {
    terminal->debugln("Failed to load settings");
  }

  // // Setup Controller and Controller Depedencies
  // Serial1.begin(PM1006K::BAUD_RATE, SERIAL_8N1, PM1006K_RX_PIN,
  // PM1006K_TX_PIN); pm1006k = new PM1006K(&Serial1);

  // // Setup I2C and BME280 Driver
  // Wire.begin(I2C_SDA, I2C_SCL);
  // bme280 = new Adafruit_BME280();
  // bme280->begin(BME280_ADDRESS, &Wire);

  // // Setup Pubsub Driver
  // pubsubClient = new PubSubClient(espClient);

  // // Setup Controller
  // controller = new REMController(&WiFi, pm1006k, bme280, pubsubClient,
  // terminal, settingsManager);

  // if (!controller->setupWiFi()) {
  //   terminal->debugln("Wifi Setup Failed");
  // }

  // // Wait about 10 seconds for the esp client to become avaliable. If it
  // doesn't,
  // // reboot the microcontroller.
  // while(!espClient.available()) {
  //   if (count > 10) {
  //     terminal->debugln("Failed to setup the esp32 networking client, trying
  //     to rebooting"); ESP.restart();
  //   }

  //   delay(1000);
  //   count ++;
  // }

  // pubsubClient->setServer(MQTT_SERVER, 1883);
  // pubsubClient->connect("arduinoClient");
  // if(controller->publish("rem/esp32/status", "online")) {
  //   terminal->debugln("Published");
  // } else {
  //   terminal->debugln("Publish Failed");
  // }

  // String config = controller->wifiConfig();
  // terminal->debugln(config);

  // // Setup Tasks
  // xTaskCreate(PublishDataTask, "Publish Data", PUBLISH_DATA_STACK, NULL, 1,
  // NULL); xTaskCreate(PublishStatusTask, "Publish Status",
  // PUBLISH_STATUS_STACK, NULL, 1, NULL);
  xTaskCreate(TerminalTask, "Terminal Task", PUBLISH_TERMINAL_STACK, NULL, 1,
              NULL);

  // Setup CLI Commands
  FreeRTOS_CLIRegisterCommand(&xRebootCommand);
  FreeRTOS_CLIRegisterCommand(&xReadEEPROMCommand);
}

// Main loop is uneeded because we are using freertos tasks to persist the
// application
void loop() { return; }

/*
 * Begin task definitions
 */

void PublishDataTask(void *paramater) {
  while (true) {
    delay(SAMPLE_RATE);
    controller->refreshPM25();
    controller->refreshBME280();
    if (!controller->publishData()) {
      terminal->debugln("Publish Data Failed");
    } else {
      terminal->debugln("Published Data");
    }
  }
}

void PublishStatusTask(void *paramater) {
  while (true) {
    delay(STATUS_RATE);
    if (!controller->publishStatus()) {
      terminal->debugln("Publish Data Failed");
    } else {
      terminal->debugln("Published Data");
    }
  }
}

void TerminalTask(void *paramater) {
  // Just handle input characters and the yield back to the core
  terminal->handleCharacter();
}

BaseType_t prvRebootCommand(char *pcWriteBuffer, size_t xWriteBufferLen,
                            const char *pcCommandString) {
  ESP.restart();

  // Never reached because the esp32 reboots fully
  return pdTRUE;
}

BaseType_t prvReadEEPROMCommand(char *pcWriteBuffer, size_t xWriteBufferLen,
                                const char *pcCommandString) {

  Serial.println("Reading EEPROM");
  Serial.write(pcWriteBuffer, xWriteBufferLen);
  Serial.println("Done");
  Serial.print(pcCommandString);

  return pdFALSE;
}