#include "WiFi.h"
#include "Wire.h"
#include "EEPROM.h"
#include "PubSubClient.h"
#include "PM1006K.h"
#include "Adafruit_BME280.h"

#include "controller.hpp"
#include "pin_config.hpp"
#include "settings_manager.hpp"
#include "terminal.hpp"

#define DEBUG false

#define PUBLISH_DATA_STACK 2048
#define PUBLISH_STATUS_STACK 1024
#define PUBLISH_TERMINAL_STACK 1024

REMController * controller;
SettingsManager * settingsManager;
PM1006K * pm1006k;
Adafruit_BME280 * bme280;
PubSubClient * pubsubClient;
WiFiClient espClient;
Terminal * terminal;

int count = 0;

// Task Defs
void PublishDataTask(void * paramater);
void PublishStatusTask(void * paramater);
void TerminalTask(void * paramater);

void setup() {
  // Turn on Fan
  pinMode(FAN, OUTPUT);
  digitalWrite(FAN, HIGH);

  // Setup debug construct
  Serial.begin(9600);
  terminal = new Terminal(DEBUG, &Serial);
  
  // Setup secrets instance
  settingsManager = new SettingsManager(terminal, &EEPROM);
  if (!settingsManager->begin()) {
    terminal->debugln("Failed to setup the eeprom");
  }

  if (!settingsManager->setWifiCredentials("password", "ssid")) {
    terminal->debugln("Failed to write data to eeprom");
  }

  delete settingsManager;
  settingsManager = new SettingsManager(terminal, &EEPROM);
  if (!settingsManager->begin()) {
    terminal->debugln("Failed to setup the eeprom");
  }

  if (!settingsManager->loadSettings()) {
    terminal->debugln("Failed to load settings");
  }

  terminal->debugln(settingsManager->getWifiPass());
  terminal->debugln(settingsManager->getWifiSSID());

  // if(!settingsManager->loadSettings()) {
  //   terminal->debugln("Failed to load secrets");
  // }

  // uint8_t * buf[126]; 
  // Serial.printf("Bytes read: %d", EEPROM.readBytes(0x00, (void *)buf, 126));

  // // Setup Controller and Controller Depedencies
  // Serial1.begin(PM1006K::BAUD_RATE, SERIAL_8N1, PM1006K_RX_PIN, PM1006K_TX_PIN);
  // pm1006k = new PM1006K(&Serial1);

  // // Setup I2C and BME280 Driver
  // Wire.begin(I2C_SDA, I2C_SCL);
  // bme280 = new Adafruit_BME280();
  // bme280->begin(BME280_ADDRESS, &Wire);

  // // Setup Pubsub Driver
  // pubsubClient = new PubSubClient(espClient);

  // // Setup Controller
  // controller = new REMController(&WiFi, pm1006k, bme280, pubsubClient, terminal, settingsManager);

  // if (!controller->setupWiFi()) {
  //   terminal->debugln("Wifi Setup Failed");
  // }

  // uint8_t count = 0;
  
  // // // Wait about 10 seconds for the esp client to become avaliable. If it doesn't, 
  // // // reboot the microcontroller.
  // // while(!espClient.available()) {
  // //   if (count > 10) {
  // //     terminal->debugln("Failed to setup the esp32 networking client, trying to rebooting");
  // //     ESP.restart();
  // //   }

  // //   delay(1000);
  // //   count ++;
  // // }

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
  // xTaskCreate(PublishDataTask, "Publish Data", PUBLISH_DATA_STACK, NULL, 1, NULL);
  // xTaskCreate(PublishStatusTask, "Publish Status", PUBLISH_STATUS_STACK, NULL, 1, NULL);
  // xTaskCreate(TerminalTask, "Terminal Task", PUBLISH_TERMINAL_STACK, NULL, 1, NULL);
}

void loop() {
  return;
}

void PublishDataTask(void * paramater) {
  while(true) {
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

void PublishStatusTask(void * paramater) {
  while(true) {
    delay(STATUS_RATE);
    if (!controller->publishStatus()) {
      terminal->debugln("Publish Data Failed");
    } else {
      terminal->debugln("Published Data");
    }
  }
}

void TerminalTask(void * paramater) {
  // Just handle input characters and the yield back to the core
  terminal->handleCharacter();
}