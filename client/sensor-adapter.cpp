// #include <Wire.h>
// #include <Adafruit_CCS811.h>
// #include <Adafruit_TSL2591.h>
// #include <Adafruit_Sensor.h>
// #include <Adafruit_BME280.h>
// #include <Adafruit_SGP30.h>


// #include "sensor-adapter.h"

// /*
//  * @brief: Returns an error if there is an issue with initializing a sensor module. 
//  * @return: 0 is an issue with the CCS811 sensor
//  */
// void SensorAdapter::init() {
//   this->_error = 0;
    
// //  if(!ccs->begin()){
// //    this->_error = 1; 
// //    return;
// //  }

//   if(!tsl->begin())
//   {
//     this->_error = 2;
//     return;
//   }

//   yield();

//   if(!bme->begin()) {
//     this->_error = 3;
//     return;
//   }

//   yield();

//   if (!sgp->begin()){
//     this->_error = 4;
//     return;
//   }

//   yield();
  
//   tsl->setGain(TSL2591_GAIN_MED);
//   tsl->setTiming(TSL2591_INTEGRATIONTIME_300MS);
// }

// SensorData SensorAdapter::getSnapshot() {
//   this->_error = 0;

//   float temp, pressure, humidity;
//   uint16_t lux;

//   sgp->IAQmeasure();
  
//   temp = bme->readTemperature();
//   yield();
  
//   pressure = bme->readPressure()/100.0F;
//   yield();

//   humidity = bme->readHumidity();
//   yield();

//   lux = tsl->getLuminosity(TSL2591_VISIBLE);
//   yield();
  
//   SensorData sd = SensorData();
//   sd.seteCO2(sgp->eCO2);
//   sd.setTVOC(sgp->TVOC);
//   sd.setLux(lux);
//   sd.setTemp(temp);
//   sd.setHumidity(humidity);
//   sd.setPressure(pressure);

//   return sd;
// }

// /*
//  * @brief: Return the error value.
//  */
// uint8_t SensorAdapter::getError() {
//   return this->_error;
// }

// SensorAdapter::SensorAdapter() {
//   this->tsl = new Adafruit_TSL2591(2591);
//   this->ccs = new Adafruit_CCS811();
//   this->bme = new Adafruit_BME280();
//   this->sgp = new Adafruit_SGP30();
// }
