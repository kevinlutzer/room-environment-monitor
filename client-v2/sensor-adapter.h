#ifndef SENSOR_ADAPTER_H
#define SENSOR_ADAPTER_H

#include <Adafruit_CCS811.h>
#include <Adafruit_TSL2561_U.h>

#include "sensor-data.h"

class SensorAdapter {
    private: 
        Adafruit_CCS811 *ccs;
        Adafruit_TSL2561_Unified *tsl;
        uint8_t _error;

    public:
        uint8_t getError();
        void init();
        SensorData getSnapshot();
        SensorAdapter();
};

#endif
