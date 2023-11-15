#include "WiFi.h"
#include "Wire.h"
#include "PubSubClient.h"
#include "PM1006K.h"
#include "Adafruit_BME280.h"

#include "controller.hpp"
#include "config.hpp"
#include "credentials.hpp"
#include "terminal.hpp"

#define DEBUG_CONTROLLER true

REMController * controller;
Credentials * credentials;
PM1006K * pm1006k;
Adafruit_BME280 * bme280;
PubSubClient * pubsubClient;
WiFiClient espClient;
Terminal * debug;

int count = 0;

void setup() {
  // Turn on Fan
  pinMode(FAN, OUTPUT);
  digitalWrite(FAN, HIGH);

  // Setup UART Logger and wait for connection
  Serial.begin(115200);
  delay(4000);

  // Setup debug construct
  debug = new Terminal(DEBUG_CONTROLLER, &Serial);
  
  // Setup secrets instance
  credentials = new Credentials(debug);
  credentials->begin();

  String wifissid = "";
  String wifipass = "";
  if(!credentials->setCredentials(wifipass, wifissid)){
    Serial.println("Failed to set credentials");
  }

  Serial.println("Loaded Secrets");
  if(!credentials->loadSecrets()) {
    Serial.println("Failed to load secrets");
  }
  Serial.println(credentials->getWifiSSID());
  Serial.println(credentials->getWifiPass());
  Serial.println("END");

  // Setup Controller and Controller Depedencies
  Serial1.begin(PM1006K::BAUD_RATE, SERIAL_8N1, PM1006K_RX_PIN, PM1006K_TX_PIN);
  pm1006k = new PM1006K(&Serial1);

  // Setup I2C and BME280 Driver
  Wire.begin(I2C_SDA, I2C_SCL);
  bme280 = new Adafruit_BME280();
  bme280->begin(BME280_ADDRESS, &Wire);

  // Setup Pubsub Driver
  pubsubClient = new PubSubClient(espClient);

  // Setup Controller
  controller = new REMController(&WiFi, pm1006k, bme280, pubsubClient, debug, credentials);

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

  // Setup Tasks
  xTaskCreate(PublishDataTask, "Publish Data", 1024, NULL, 1, NULL);
  xTaskCreate(PublishStatusTask, "Publish Status", 1024, NULL, 1, NULL);
}

void loop() {}

//
// Tasks
// 

void PublishDataTask(void * paramater) {
  while(true) {
    delay(SAMPLE_RATE);
    controller->refreshPM25();
    controller->refreshBME280();
    if (!controller->publishData()) {
      Serial.println("Publish Data Failed");
    } else {
      Serial.println("Published Data");
    }
  }
}

void PublishStatusTask(void * paramater) {
  while(true) {
    delay(STATUS_RATE);
    if (!controller->publishStatus()) {
      Serial.println("Publish Data Failed");
    } else {
      Serial.println("Published Data");
    }
  }
}