#ifndef SENSORDATA_H
#define SENSORDATA_H

class SensorData {
    public:
        uint16_t geteCO2();
        uint16_t getTVOC();
        SensorData(uint16_t eCO2, uint16_t TVOC) {
          _eCO2 = eCO2;
          _TVOC = TVOC;
        };

    private:
      uint16_t _eCO2, _TVOC;
};

#endif
