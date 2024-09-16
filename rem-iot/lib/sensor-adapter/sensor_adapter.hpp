#ifndef _REM_CONTROLLER_H
#define _REM_CONTROLLER_H

#include "Adafruit_BME280.h"
#include "PM1006K.h"
#include "PubSubClient.h"
#include "UUID.h"
#include "WiFi.h"
#include "time.h"

#include "settings_manager.hpp"
#include "terminal.hpp"

// The amount of characters needed for each of the settings, their names, and some formating character (4)
#define SENSOR_PRINT_LEN 73 + 3 * sizeof(int) + 3 * sizeof(double)

/**
 * @brief SensorAdapter is responsible for fetching the latest sensor data from
 * each of the peripheral devices. The data is then accessible within the class
 * instance via getter methods.
 *  */
class SensorAdapter {
public:
  SensorAdapter(PM1006K *pm1006k, Adafruit_BME280 *bme280, Terminal *terminal);

  /**
   * @brief Calls each of the sensor libraries to grab the latest data. The data
   * then becomes available on the class instance.
   * @return bool True if the pm1006k specific data was loaded, false otherwise.
   * Note that we don't have a failure case to worry about for the bme280.
   */
  bool loadData();

  /** 
   * @brief Print the latest sensor data to the buffer. The buffer must be at least
   * SENSOR_PRINT_LEN in length.
   * @return bool True if the `buf_len` is long enough, false otherwise.
   */
  bool printData(char *buf, size_t buf_len);

  // Getters for the instances local data
  int getPM2_5() { return this->pm2_5; }
  int getPM1_0() { return this->pm1_0; }
  int getPM10() { return this->pm10; }

  float getTemperature() { return this->temperature; }
  float getPressure() { return this->pressure; }
  float getHumidity() { return this->humidity; }

private:
  // Different service providers
  PM1006K *pm1006k;
  Adafruit_BME280 *bme280;
  Terminal *terminal;

  // Latest data values
  int pm2_5 = -1;
  int pm1_0 = -1;
  int pm10 = -1;

  float temperature = 0;
  float pressure = 0;
  float humidity = 0;
};

#endif