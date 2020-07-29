#include <stdint.h>
#include <String>
#include <cstdio>
#include <iostream>

#include "sensor-data.h"

using namespace std;

char* SensorData::stringify(void) {
  char *str = "";
  sprintf(str, "{\"lux\":%u,\"pressure\":%d,\"humidity\":%d,\"room_temp\":%d,\"co2\":%u,\"tvoc\":%d}", this->_Lux, this->_Pressure, this->_Humidity, this->_Temp, this->_eCO2, this->_TVOC);
  return str;
}

void SensorData::seteCO2(uint16_t eCO2) {
  this->_eCO2 = eCO2;
}

void SensorData::setTVOC(uint16_t TVOC) {
  this->_TVOC = TVOC;
}

void SensorData::setLux(float Lux) {
  this->_Lux = Lux;
}

void SensorData::setTemp(float Temp) {
  this->_Temp = Temp;  
}

void SensorData::setHumidity(float Humidity) {
  this->_Humidity = Humidity;  
}

void SensorData::setPressure(float Pressure) {
  this->_Pressure = Pressure;
}
