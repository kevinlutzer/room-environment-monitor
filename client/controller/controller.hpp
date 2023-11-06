#ifndef _REM_CONTROLLER_H
#define _REM_CONTROLLER_H

#include "pm1006.hpp"
#include "sensordata.hpp"
#include "Adafruit_BME280.h"
#include "PubSubClient.h"
#include "WiFi.h"
#include "time.h"

class REMController {
    public:
        REMController(WiFiClass *wifi, PM1006 *pm1006, Adafruit_BME280 *bme280, PubSubClient *pubsubClient);
        bool setupWiFi();
        bool refreshPM25();
        bool refreshBME280();
        bool setupSNTP();
        bool verifyClockSync();
        bool publish(const char * topic, const char * payload);
        bool publishData();
        bool publishStatus();
        String wifiConfig();

    private:
        // Drivers
        PM1006 *pm1006;
        WiFiClass *wifi;
        Adafruit_BME280 *bme280;
        PubSubClient *pubsubClient;
        
        // Data
        PM1006Data * pm1006Data;
        BME280Data * bme280Data;
};

#endif