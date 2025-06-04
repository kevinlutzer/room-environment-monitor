

#include "tasks.hpp"
#include "mqtt_msg.hpp"

// Create a queue to handle the messages that are sent to the MQTT server
static QueueHandle_t msgQueue = xQueueCreate(10, sizeof(MQTTMsg *));

void applySensorData(MQTTMsg *mqttMsg, SensorAdapter *sensorAdapter) {
  // Apply PM1006K data to the msg
  mqttMsg->setField("pm2_5", sensorAdapter->getPM2_5());
  mqttMsg->setField("pm1_0", sensorAdapter->getPM1_0());
  mqttMsg->setField("pm10", sensorAdapter->getPM10());

  // Apply BMP280 data to the msg
  mqttMsg->setField("temperature", sensorAdapter->getTemperature());
  mqttMsg->setField("humidity", sensorAdapter->getHumidity());
  mqttMsg->setField("pressure", sensorAdapter->getPressure());

  // Apply SGP40 voc index
  mqttMsg->setField("vocIndex", sensorAdapter->getVocIndex());
}

void QueueDataTask(void *parameter) {
  REMTaskProviders *providers = (REMTaskProviders *)parameter;
  while (true) {
    providers->uuidGenerator->generate();

    const char *deviceId = providers->settingsManager->getSetting(DEVICE_ID_ID);
    const char *dataTopic =
        providers->settingsManager->getSetting(DATA_TOPIC_ID);
    MQTTMsg *msg = new MQTTMsg(dataTopic, deviceId,
                               providers->uuidGenerator->toCharArray());
    if (msg == NULL) {
      providers->terminal->debugln(
          "Failed to create msg, there is not enough memory");
      return;
    }

    // If we can get pm1006 data, add it to the msg
    if (!providers->sensorAdapter->loadData()) {
      providers->terminal->debugln("Failed to take measurement");
    }

    applySensorData(msg, providers->sensorAdapter);

    providers->terminal->debugf("Queing message with id %s /r/n", msg->getId());

    if (xQueueSend(msgQueue, (void *)&msg, MSG_QUEUE_TIMEOUT) ==
        errQUEUE_FULL) {
      providers->terminal->debugln("Msg queue is full");
      return;
    }

    providers->terminal->debugln("Queued latest sensor data");
    delay(DATA_SAMPLE_RATE);
  }
}

void applyStatusInformation(MQTTMsg *mqttMsg, REMTaskProviders *providers) {
  struct tm currentTime;
  double uptime = esp_timer_get_time() / 1000; // Convert to milliseconds

  mqttMsg->setField("uptime", uptime);
  mqttMsg->setField("rssi", WiFi.RSSI());
}

void QueueStatusTask(void *parameter) {
  REMTaskProviders *providers = (REMTaskProviders *)parameter;

  // Start the status tasks loop
  while (true) {

    providers->uuidGenerator->generate();

    const char *deviceId = providers->settingsManager->getSetting(DEVICE_ID_ID);
    const char *statusTopic =
        providers->settingsManager->getSetting(STATUS_TOPIC_ID);
    MQTTMsg *msg = new MQTTMsg(statusTopic, deviceId,
                               providers->uuidGenerator->toCharArray());
    if (msg == NULL) {
      providers->terminal->debugln(
          "Failed to create msg, there is not enough memory");
      continue;
    }

    if (xQueueSend(msgQueue, (void *)&msg, MSG_QUEUE_TIMEOUT) ==
        errQUEUE_FULL) {
      providers->terminal->debugln("Msg queue is full");
      continue;
    }

    applyStatusInformation(msg, providers);

    providers->terminal->debugln("Queued status");

    // Delay for the status sample rate
    delay(STATUS_SAMPLE_RATE);
  }
}

void PublishMQTTMsg(void *parameter) {
  REMTaskProviders *providers = (REMTaskProviders *)parameter;
  MQTTMsg *msg;

  while (true) {
    // Wait for a message to be received from the queue indefinitely
    // note that this doesn't actually block.

    if (xQueueReceive(msgQueue, (void *)&msg, portMAX_DELAY) == pdTRUE) {
      providers->terminal->debugf("Received message from queue with ID: %s\r\n",
                                  msg->getId());

      // Serialize the message to a buffer
      char output[MAX_DOC_SIZE];
      msg->serialize(output);

      // Check if the pubsub client is connected, if not, try to connect
      while (!providers->pubSubClient->connected()) {
        providers->terminal->debugln(
            "Failed to connect to MQTT server, retrying...");
        providers->pubSubClient->connect("arduinoClient");
        delay(1000);
      }

      if (!providers->pubSubClient->publish(msg->getTopic(), output)) {
        providers->terminal->debugln("Failed to publish message");
      } else {
        providers->terminal->debugln("Published message");
      }

      // Assume that all messages passed to the queue must be deleted after
      // they are published to the MQTT server here.
      delete msg;
    }

    // Yield to the core to allow other tasks to run
    // This task should be queued with a lower priority than the other tasks
    delay(TERMINAL_SAMPLE_RATE);
  }
}

void TerminalTask(void *parameter) {
  Terminal *terminal = (Terminal *)parameter;

  // Just handle input characters and the yield back to the core
  terminal->handleCharacter();
}

void LEDUpdateTask(void *parameter) {
  REMTaskProviders *providers = (REMTaskProviders *)parameter;

  while (true) {

    // Get the latest sensor data
    if (!providers->sensorAdapter->loadData()) {
      providers->terminal->debugln("Failed to take measurement");
    }

    // Get the PM values, if anyone of these values is bad
    double pm2_5 = providers->sensorAdapter->getPM2_5();
    double pm1_0 = providers->sensorAdapter->getPM1_0();
    double pm10 = providers->sensorAdapter->getPM10();

    struct tm currentTime;
    if (!getLocalTime(&currentTime)) {
      providers->terminal->debugln("Failed to get current time");
      goto delayInLoop;
    }

    // Check if the current time is between 6am and 8pm
    if (currentTime.tm_hour < 6 || currentTime.tm_hour > 20) {
      providers->terminal->debugf("Turning off LED panel because it is between "
                                  "8pm and 6am, hour is: %d\r\n",
                                  currentTime.tm_hour);
      providers->ledController->clear();
      goto delayInLoop;
    }

    // Just update based on the PM values now if we are in the time range
    providers->ledController->updateFromPM(pm2_5, pm1_0, pm10);
    providers->terminal->debugln("Updating LED panel based on the PM values");

  delayInLoop:
    delay(LED_UPDATE_RATE);
  }
}

#ifdef DEBUG_MEMORY_CHECK

void MemoryCheck(void *parameter) {
  REMTaskProviders *providers = (REMTaskProviders *)parameter;

  while (true) {
    // Total free heap
    providers->terminal->debugf("Free heap: %u bytes\n", ESP.getFreeHeap());

    // Largest block of memory that can be allocated
    providers->terminal->debugf(
        "Largest free block: %u bytes\n",
        heap_caps_get_largest_free_block(MALLOC_CAP_8BIT));

    // Minimum free heap ever recorded (good for checking overflows)
    providers->terminal->debugf("Minimum free heap: %u bytes\n",
                                ESP.getMinFreeHeap());

    // Print the free heap size every 10 seconds
    providers->terminal->debugf("Free heap size: %d bytes\r\n",
                                esp_get_free_heap_size());
    delay(1000);
  }
}

#endif // DEBUG_MEMORY_CHECK
