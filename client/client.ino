#include "WiFi.h"
#include "Wire.h"
#include "pm1006.hpp"
#include "controller.hpp"

REMController * controller;
PM1006 * pm1006;
Adafruit_BME280 * bme280;

int count = 0;

void setup() {

  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);

  // Setup UART Logger
  Serial.begin(115200);
  
  // Setup Controller and Controller Depedencies
  pm1006 = new PM1006(&Serial1, 4, 5); 
  pm1006->begin();

  Wire.begin(7, 6);
  bme280 = new Adafruit_BME280();

  delay(4000);
  if(!bme280->begin(0x77, &Wire)) {
    Serial.println("Couldn't find the BME280 sensor");
  } else {
    Serial.println("Sensor found");
  }

  controller = new REMController(&WiFi, pm1006, bme280, 10);

  if (!controller->setupWiFi()) {
    Serial.println("Failed");
  }

  String config = controller->wifiConfig();
  Serial.println(config);

  // Initialize FreeRTOS Tasks
  xTaskCreate(
    RefreshPM25Task
    ,  "Refresh PM25"
    ,  2048  // Stack size
    ,  NULL
    ,  1  // Low priority
    ,  NULL // Task handle is not used here - simply pass NULL
  );
}

void loop() {
  delay(3000);
  Serial.print("Temperature = ");
  Serial.print(bme280->readTemperature());
  Serial.println(" *C");
  Serial.print("Pressure = ");
  Serial.println(bme280->readPressure());
  Serial.print("Humidity = "); 
  Serial.println(bme280->readHumidity());
  Serial.println("");
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void RefreshPM25Task(void * parameter) {
  while(true) {
    controller->refreshPM25();
    delay(4000);
  }
}