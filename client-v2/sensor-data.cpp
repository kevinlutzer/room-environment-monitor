#include <stdint.h>
#include "sensor-data.h"

/*
 * @brief: Return the eCO2 value.
 */
uint16_t SensorData::geteCO2() {
  return this->_eCO2;
}

/*
 * @brief: Return the TVOC value.
 */
uint16_t SensorData::getTVOC() {
  return this->_TVOC;
}
