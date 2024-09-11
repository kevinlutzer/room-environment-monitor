#ifndef _TASKS_HPP
#define _TASKS_HPP

#include "PubSubClient.h"
#include "Arduino.h"
#include "sensor_adapter.hpp"
#include "settings_manager.hpp"
#include "terminal.hpp"

#define DATA_SAMPLE_RATE 5000
#define STATUS_SAMPLE_RATE 1000

/**
 * @brief REMTaskProviders is a class that holds all of the different services/instances that the different FreeRTOS tasks need to run.
 * Note that the instance of this class is passed to the tasks via a null pointer, and then the tasks just does an implicit cast to this class type.
 */
class REMTaskProviders {

public:
    REMTaskProviders(SensorAdapter *sensorAdapter, SettingsManager *settingsManager, Terminal *terminal, PubSubClient * pubSubClient, UUID * uuidGenerator) {
        this->sensorAdapter = sensorAdapter;
        this->settingsManager = settingsManager;
        this->terminal = terminal;
        this->pubSubClient = pubSubClient;
        this->msgQueue = msgQueue;
        this->uuidGenerator = uuidGenerator;
    }

    SensorAdapter *sensorAdapter;
    SettingsManager *settingsManager;
    Terminal *terminal;
    PubSubClient * pubSubClient;
    UUID * uuidGenerator;
    QueueHandle_t * msgQueue;
};

/**
 * @brief QueueDataTask is a task that queues the latest sensor data to be sent to the MQTT server.
 * Note that it does not actually publish the data.
 */
void QueueDataTask(void *paramater);

/**
 * @brief QueueStatusTask is a task that queues the status of the device to be sent to the MQTT server.
 */
void QueueStatusTask(void *paramater);

/**
 * @brief PublishMQTTMsg is a queue worker that takes all queued MQTT messages and publishes them via 
 * the pubsub client.
 */
void PublishMQTTMsg(void *paramater);

/**
 * @brief TerminalTask is a task that handles the terminal input and output.
 */
void TerminalTask(void *paramater);

#endif


