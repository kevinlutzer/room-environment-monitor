#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <string>

using namespace std;

class SensorData {
    public:
        SensorData(uint16_t pm2_5, uint16_t tvoc, float temp, float humidity, float pressure);
        char * stringify(void);

    private:
      uint16_t _eCO2, _TVOC, _spgeCO2, _spgTVOC;
      float _Lux, _Temp, _Humidity, _Pressure;
};

#endif
