#include <stdint.h>
#include <String>
#include <cstdio>
#include <iostream>

#include "sensor-data.h"

using namespace std;

/*
 * @brief Return the eCO2 value.
 */
uint16_t SensorData::geteCO2() {
  return this->_eCO2;
}

/*
 * @brief Return the TVOC value.
 */
uint16_t SensorData::getTVOC() {
  return this->_TVOC;
}

/*
 * @brief Return the Lux value
*/
float SensorData::getLux() {
  return this->_Lux;
}

float SensorData::getTemp() {
  return this->_Temp;
}

float SensorData::getHumidity() {
  return this->_Humidity;
}

float SensorData::getPressure() {
  return this->_Pressure;  
}

/*
 *  Lux       uint32    `json:"lux"`
  Pressure  float32   `json:"pressure"`
  Humidity  float32   `json:"humidity"`
  RoomTemp  float32   `json:"room_temp"`
  CO2       uint16    `json:"co2"`
  TVOC      uint16    `json:"tvoc"`
  CPUTemp   float32   `json:"cpu_temp"`
  TimeStamp time.Time `json:"timestamp"`
 */

char* SensorData::stringify(void) {
  char *str = "";
  sprintf(str, "{\"lux\":%u,\"pressure\":%d,\"humidity\":%d,\"room_temp\":%d,\"co2\":%u,\"tvoc\":%d}", this->_Lux, this->_Pressure, this->_Humidity, this->_Temp, this->_eCO2, this->_TVOC);
  return str;
}
