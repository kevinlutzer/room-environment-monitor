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

/**
 * @brief REMController is responsible for creating and queuing telemetry data and status messages in the form of a MQTTMsg. The queued data will 
 * be consumed by a queue worker that will publish each message 
 *  */ 
class REMController {
    public:
        REMController(PM1006K *pm1006k, Adafruit_BME280 *bme280, Terminal * terminal, SettingsManager * settingsManager, QueueHandle_t * msgQueue);
        ~REMController();

        /**
         * @brief Send the latest sensor data to the `msgQueue`.
         */
        void queueLatestSensorData();

        /**
         * @brief Send the status of the device to the `msgQueue`.
         */
        void queueStatus();

    private:

        // Different service providers
        PM1006K *pm1006k;
        Adafruit_BME280 *bme280;
        Terminal * terminal;
        SettingsManager * settingsManager;
        QueueHandle_t * msgQueue;
        
        // UUID generator is used for generating unique ids for each message
        UUID *uuidGenerator;

        // Start time to track uptime of the device
        struct tm * startTime;
        emperature_sensor_handle_t * tempHandle;
};

#endif