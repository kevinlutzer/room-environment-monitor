#include <stdint.h>
#include <String>
#include <cstdio>
#include <iostream>

#include "SensorData.h"

using namespace std;

SensorData(uint16_t pm2_5, uint16_t tvoc, float temp, float humidity, float pressure) {
    this->_eCO2 = pm2_5;
    this->_TVOC = tvoc;
    this->_Temp = temp;
    this->_Humidity = humidity;
    this->_Pressure = pressure;
}

char* SensorData::stringify(void) {
  char *str = "";
  sprintf(str, "{\"lux\":%u,\"pressure\":%d,\"humidity\":%d,\"room_temp\":%d,\"co2\":%u,\"tvoc\":%d}", this->_Lux, this->_Pressure, this->_Humidity, this->_Temp, this->_eCO2, this->_TVOC);
  return str;
}

