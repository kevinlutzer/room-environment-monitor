#include <stdint.h>
#include "sensor-data.h"

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
