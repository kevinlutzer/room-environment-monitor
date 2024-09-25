#ifndef _REM_PROVIDERS_HPP
#define _REM_PROVIDERS_HPP

#include "PubSubClient.h"
#include "Arduino.h"
#include "led_controller.hpp"
#include "sensor_adapter.hpp"
#include "settings_manager.hpp"
#include "terminal.hpp"
#include "led_controller.hpp"

/**
 * @brief REMProviders is a class that holds all of the different services/instances that the different FreeRTOS tasks and FreeRTOS_CLI commands need to run.
 * Note that the instance of this class is passed to the tasks via a null pointer, and then the tasks just does an implicit cast to this class type.
 */
class REMProviders {

public:
    REMProviders(SensorAdapter *sensorAdapter, SettingsManager *settingsManager, Terminal *terminal, PubSubClient * pubSubClient, UUID * uuidGenerator, LEDController * ledController) {
        this->sensorAdapter = sensorAdapter;
        this->settingsManager = settingsManager;
        this->terminal = terminal;
        this->pubSubClient = pubSubClient;
        this->uuidGenerator = uuidGenerator;
        this->ledController = ledController;
    }

    SensorAdapter *sensorAdapter;
    SettingsManager *settingsManager;
    Terminal *terminal;
    PubSubClient * pubSubClient;
    UUID * uuidGenerator;
    LEDController * ledController;
};

#endif