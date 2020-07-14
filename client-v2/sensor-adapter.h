#ifndef SENSOR_ADAPTER_H
#define SENSOR_ADAPTER_H

#include "Adafruit_CCS811.h"
#include "sensor-data.h"

class SensorAdapter {
    private: 
        Adafruit_CCS811 ccs;

    public:
        SensorData get_snapshot();
};

#endif
