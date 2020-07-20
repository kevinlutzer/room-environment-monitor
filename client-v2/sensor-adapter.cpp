#include <Adafruit_CCS811.h>
#include <Adafruit_TSL2561_U.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include "sensor-adapter.h"

/*
 * @brief: Returns an error if there is an issue with initializing a sensor module. 
 * @return: 0 is an issue with the CCS811 sensor
 */
void SensorAdapter::init() {
  this->_error = 0;
    
//  if(!ccs->begin()){
//    this->_error = 1; 
//    return;
//  }

  if(!tsl->begin())
  {
    this->_error = 2;
    return;
  }

  if(!bme->begin()) {
    this->_error = 3;
    return;
  }

  tsl->enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */
  tsl->setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
}

SensorData SensorAdapter::getSnapshot() {
  this->_error = 0;

  int counter = 0;
  while(!ccs->available()) {
    yield();
    if (counter == 2) {
      this->_error = 1;
      return SensorData(0,0,0,0,0,0);
    }
    counter ++;
  }
  
  if (ccs->readData()) {
    this->_error = 1;
  }
  yield();

  float temp, pressure, humidity;
  temp = bme->readTemperature();
  yield();
  pressure = bme->readPressure()/100.0F;
  humidity = bme->readHumidity();

  sensors_event_t event;
  tsl->getEvent(&event);
  yield();

  return SensorData(this->ccs->geteCO2(),this->ccs->getTVOC(), event.light, temp, humidity, pressure);
}

/*
 * @brief: Return the error value.
 */
uint8_t SensorAdapter::getError() {
  return this->_error;
}

SensorAdapter::SensorAdapter() {
  this->tsl = new Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);
  this->ccs = new Adafruit_CCS811();
  this->bme = new Adafruit_BME280();
}
