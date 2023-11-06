#include "WiFi.h"
#include "Wire.h"

#include "pm1006.hpp"
#include "controller.hpp"
#include "PubSubClient.h"
#include "Config.hpp"

REMController * controller;
PM1006 * pm1006;
Adafruit_BME280 * bme280;
PubSubClient * pubsubClient;
WiFiClient espClient;

#define SAMPLE_RATE 30000 // ms
#define STATUS_RATE 10000 // ms

int count = 0;

void setup() {
  // Turn on Fan
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);

  // Setup UART Logger and wait for connection
  Serial.begin(115200);
  delay(4000);
  
  // Setup Controller and Controller Depedencies
  pm1006 = new PM1006(&Serial1, 4, 5); 
  pm1006->begin();

  // Setup I2C and BME280 Driver
  Wire.begin(7, 6);
  bme280 = new Adafruit_BME280();
  bme280->begin(0x77, &Wire);

  // Setup Pubsub Driver
  pubsubClient = new PubSubClient(espClient);
  controller = new REMController(&WiFi, pm1006, bme280, pubsubClient);

  if (!controller->setupWiFi()) {
    Serial.println("Wifi Setup Failed");
  }

  pubsubClient->setServer(MQTT_SERVER, 1883);
  if (!espClient.available()) {
    Serial.println("Client Not Available");
  }

  pubsubClient->connect("arduinoClient");
  if(controller->publish("rem/esp32/status", "online")) {
    Serial.println("Published");
  } else {

    Serial.println("Publish Failed");
  }

  
  String config = controller->wifiConfig();
  Serial.println(config);
}

void PublishDataTask(void * paramater) {
  delay(SAMPLE_RATE);
  controller->refreshPM25();
  controller->refreshBME280();
  if (!controller->publishData()) {
    Serial.println("Publish Data Failed");
  } else {
    Serial.println("Published Data");
  }
}

void loop() {
  delay(STATUS_RATE);
  if (!controller->publishStatus()) {
    Serial.println("Publish Status");
  } else {
    Serial.println("Published Status");
  }
}