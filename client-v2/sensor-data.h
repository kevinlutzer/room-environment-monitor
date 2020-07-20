#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <string>

using namespace std;

class SensorData {
    public:
        uint16_t geteCO2();
        uint16_t getTVOC();
        float getLux();
        float getTemp();
        float getHumidity();
        float getPressure();
        SensorData(uint16_t eCO2, uint16_t TVOC, float Lux, float Temp, float Humidity, float Pressure) {
          _eCO2 = eCO2;
          _TVOC = TVOC;
          _Lux = Lux;
          _Temp = Temp;
          _Humidity = Humidity;
          _Pressure = Pressure;
        };
        char * stringify(void);

    private:
      uint16_t _eCO2, _TVOC;
      float _Lux, _Temp, _Humidity, _Pressure;
};

#endif
