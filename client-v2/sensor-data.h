#ifndef SENSORDATA_H
#define SENSORDATA_H

class SensorData {
    public:
        uint16_t eC02, TV0C;

        void constructor() {
          eC02 = 0; TV0C = 0;
        }

};

#endif
