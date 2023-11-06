#include "time.h"

class SensorData {
    public:
        struct tm getLastUpdate() {
            return lastUpdate;
        };

        SensorData * setLastUpdate(struct tm lastUpdate) {
            this->lastUpdate = lastUpdate;
            return this;
        };

    private:
          struct tm lastUpdate;
};

class PM1006Data : public SensorData {
    public:
        int getPM25() {
            return this->pm25;
        };

        void setPM25(int pm25) {
            this->pm25 = pm25;
        };

    private:
        int pm25;
};

class BME280Data : public SensorData {
    public:
        float getHumidity() {
            return this->humidity;
        };

        float getTemperature() {
            return this->temperature;
        };

        float getPressure() {
            return this->pressure;
        };

        BME280Data * setHumidity(float humidity) {
            this->humidity = humidity;
            return this;
        };

        BME280Data * setTemperature(float temperature) {
            this->temperature = temperature;
            return this;
        };

        BME280Data * setPressure(float pressure) {
            this->pressure = pressure;
            return this;
        };

    private:
        float humidity;
        float temperature;
        float pressure;
};