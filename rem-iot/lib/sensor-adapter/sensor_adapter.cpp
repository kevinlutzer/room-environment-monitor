#include "Adafruit_BME280.h"
#include "Adafruit_SGP40.h"
#include "Arduino.h"
#include "ArduinoJson.h"
#include "PubSubClient.h"
#include "UUID.h"
#include "WiFi.h"

#include "mqtt_msg.hpp"
#include "pin_config.hpp"
#include "sensor_adapter.hpp"
#include "settings_manager.hpp"
#include "terminal.hpp"

SensorAdapter::SensorAdapter(PM1006K *pm1006k, Adafruit_BME280 *bme280,
                             Adafruit_SGP40 *sgp40, Terminal *terminal) {
  this->pm1006k = pm1006k;
  this->bme280 = bme280;
  this->sgp40 = sgp40;
  this->terminal = terminal;
}

bool SensorAdapter::printData(char *buf, size_t buf_len) {
  if (buf_len < SENSOR_PRINT_LEN) {
    return false;
  }

  snprintf(buf, buf_len,
           "\r\npm1_0: %d\r\npm2_5: %d\r\npm10: %d\r\ntemperature: "
           "%f\r\npressure: %f\r\nhumidity: %f\r\n",
           this->pm1_0, this->pm2_5, this->pm10, this->temperature,
           this->pressure, this->humidity);

  return true;
}

bool SensorAdapter::loadData() {
  bool success = true;

  // Manually call the pm1006k to take a measurement
  if (!this->pm1006k->takeMeasurement()) {
    this->terminal->debugln("Failed to take measurement");
    success = false;
  }

  // Apply PM1006K data to the class instance, it may be invalid
  // if the takeMeasurement() failed
  this->pm2_5 = this->pm1006k->getPM2_5();
  this->pm1_0 = this->pm1006k->getPM1_0();
  this->pm10 = this->pm1006k->getPM10();

  // Apply BMP280 data to the class instance
  this->temperature = this->bme280->readTemperature();
  this->humidity = this->bme280->readHumidity();
  this->pressure = this->bme280->readPressure();

<<<<<<< Updated upstream
  // Get the VOC index from the SGP40 from the temperature and humidity data
  // fetched from the BME280 sensor
  this->vocIndex =
      this->sgp40->measureVocIndex(this->temperature, this->humidity);
=======
  // Load sensor data from the 
  #ifdef HAS_SGP40
    this->sgp40->
  #endif
>>>>>>> Stashed changes

  return success;
}

int SensorAdapter::getPM2_5() { return this->pm2_5; }

int SensorAdapter::getPM1_0() { return this->pm1_0; }

int SensorAdapter::getPM10() { return this->pm10; }

float SensorAdapter::getTemperature() { return this->temperature; }

float SensorAdapter::getPressure() { return this->pressure; }

float SensorAdapter::getHumidity() { return this->humidity; }

int32_t SensorAdapter::getVocIndex() { return this->vocIndex; }