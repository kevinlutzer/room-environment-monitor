#include <Adafruit_CCS811.h>
#include <Adafruit_TSL2561_U.h>
#include <Adafruit_Sensor.h>

#include "sensor-adapter.h"

/*
 * @brief: Returns an error if there is an issue with initializing a sensor module. 
 * @return: 0 is an issue with the CCS811 sensor
 */
void SensorAdapter::init() {
  if(!ccs->begin()){
    this->_error = 1; 
  }

  this->_error = 0;
}

SensorData SensorAdapter::getSnapshot() {
  this->_error = 0;

  int counter = 0;
  while(!ccs->available()) {
    delay(1000);
    if (counter == 2) {
      this->_error = 1;
      return SensorData(0,0);
    }
    counter ++;
  }
  
  if (ccs->readData()) {
    this->_error = 1;
  }

  return SensorData(this->ccs->geteCO2(),this->ccs->getTVOC());
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
}
