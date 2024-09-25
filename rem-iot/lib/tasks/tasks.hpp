#ifndef _TASKS_HPP
#define _TASKS_HPP

#include "PubSubClient.h"
#include "Arduino.h"
#include "led_controller.hpp"
#include "sensor_adapter.hpp"
#include "settings_manager.hpp"
#include "terminal.hpp"
#include "led_controller.hpp"

#define DATA_SAMPLE_RATE 5000
#define STATUS_SAMPLE_RATE 1000
#define TERMINAL_SAMPLE_RATE 10
#define LED_UPDATE_RATE 2000

// How long to wait for message in queue to be received before a timeout
#define MSG_QUEUE_TIMEOUT pdMS_TO_TICKS(10) // 10ms

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

/**
 * @brief LEDUpdateTask updates the led pannel (3 neopixels) based on the latest sensor data. Green mains the
 * air quality is good, yellow means the air quality is moderate, and red means the air quality is bad.
 */
void LEDUpdateTask(void *parameter);

#endif


