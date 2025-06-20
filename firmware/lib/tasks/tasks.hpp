#ifndef _TASKS_HPP
#define _TASKS_HPP

#include "Arduino.h"
#include "PubSubClient.h"
#include "led_controller.hpp"
#include "sensor_adapter.hpp"
#include "settings_manager.hpp"
#include "terminal.hpp"

#define DATA_SAMPLE_RATE 60000
#define STATUS_SAMPLE_RATE 120000
#define TERMINAL_SAMPLE_RATE 10
#define LED_UPDATE_RATE 60000 

// #define DEBUG_MEMORY_CHECK true

// How long to wait for message in queue to be received before a timeout
#define MSG_QUEUE_TIMEOUT pdMS_TO_TICKS(10) // 10ms

/**
 * @brief REMTaskProviders is a class that holds all of the different
 * services/instances that the different FreeRTOS tasks need to run. Note that
 * the instance of this class is passed to the tasks via a null pointer, and
 * then the tasks just does an implicit cast to this class type.
 */
class REMTaskProviders {

public:
  REMTaskProviders(SensorAdapter *sensorAdapter,
                   SettingsManager *settingsManager, Terminal *terminal,
                   PubSubClient *pubSubClient, UUID *uuidGenerator,
                   LEDController *ledController) {
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
  PubSubClient *pubSubClient;
  UUID *uuidGenerator;
  LEDController *ledController;
};

/**
 * @brief QueueDataTask is a task that queues the latest sensor data to be sent
 * to the MQTT server. Note that it does not actually publish the data.
 */
void QueueDataTask(void *paramater);

/**
 * @brief QueueStatusTask is a task that queues the status of the device to be
 * sent to the MQTT server.
 */
void QueueStatusTask(void *paramater);

/**
 * @brief PublishMQTTMsg is a queue worker that takes all queued MQTT messages
 * and publishes them via the pubsub client.
 */
void PublishMQTTMsg(void *paramater);

/**
 * @brief TerminalTask is a task that handles the terminal input and output.
 */
void TerminalTask(void *paramater);

/**
 * @brief LEDUpdateTask updates the led pannel (3 neopixels) based on the latest
 * sensor data. Green mains the air quality is good, yellow means the air
 * quality is moderate, and red means the air quality is bad.
 */
void LEDUpdateTask(void *parameter);

/**
 * @brief MemoryCheck is a task that checks the free memory and prints it to the
 * terminal.
 */
#ifdef DEBUG_MEMORY_CHECK
void MemoryCheck(void *parameter);
#endif // DEBUG_MEMORY_CHECK

#endif
