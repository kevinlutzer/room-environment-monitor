#ifndef _REM_CONTROLLER_H
#define _REM_CONTROLLER_H

#include "Adafruit_BME280.h"
#include "PubSubClient.h"
#include "WiFi.h"
#include "time.h"
#include "PM1006K.h"
#include "UUID.h"

#include "terminal.hpp"
#include "settings_manager.hpp"

#define MSG_QUEUE_TIMEOUT pdMS_TO_TICKS(10) // 10ms 

class REMController {
    public:
        REMController(PM1006K *pm1006k, Adafruit_BME280 *bme280, Terminal * terminal, SettingsManager * settingsManager, QueueHandle_t * msgQueue);
        void queueLatestSensorData();
        void queueStatus();

    private:
        // Drivers
        PM1006K *pm1006k;
        Adafruit_BME280 *bme280;
        Terminal * terminal;
        SettingsManager * settingsManager;
        QueueHandle_t * msgQueue;
        
        // Utilities
        UUID *uuidGenerator;
};

#endif