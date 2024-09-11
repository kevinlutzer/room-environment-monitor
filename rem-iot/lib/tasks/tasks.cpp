

#include "tasks.hpp"
#include "mqtt_msg.hpp"

// Create a queue to handle the messages that are sent to the MQTT server
static QueueHandle_t msgQueue = xQueueCreate(10, sizeof(MQTTMsg *));

void QueueDataTask(void *parameter) {
  REMTaskProviders * providers = (REMTaskProviders *)parameter;
  while (true) {    
    providers->uuidGenerator->generate();

    const char * deviceId  = providers->settingsManager->getSetting(DEVICE_ID_ID);
    const char * dataTopic = providers->settingsManager->getSetting(DATA_TOPIC_ID);
    MQTTMsg * msg = new MQTTMsg(dataTopic, deviceId, providers->uuidGenerator->toCharArray());   
    if (msg == NULL) {
        providers->terminal->debugln("Failed to create msg, there is not enough memory");
        return;
    }

    // If we can get pm1006 data, add it to the msg
    if (!providers->sensorAdapter->loadData()) {
        providers->terminal->debugln("Failed to take measurement");
        // return;
    }

    // Apply PM1006K data to the msg
    msg->setField("pm2_5", providers->sensorAdapter->getPM2_5());
    msg->setField("pm1_0", providers->sensorAdapter->getPM1_0());
    msg->setField("pm10", providers->sensorAdapter->getPM10());

    // Apply BMP280 data to the msg
    msg->setField("temperature", providers->sensorAdapter->getTemperature());
    msg->setField("humidity", providers->sensorAdapter->getHumidity());
    msg->setField("pressure", providers->sensorAdapter->getPressure());

    if ( xQueueSend( msgQueue, ( void * ) &msg, MSG_QUEUE_TIMEOUT ) == errQUEUE_FULL) {
        providers->terminal->debugln("Msg queue is full");
        return;
    }

    providers->terminal->debugln("Queued latest sensor data");
    delay(DATA_SAMPLE_RATE);
  }
}

void QueueStatusTask(void *parameter) {
  REMTaskProviders * providers = (REMTaskProviders *)parameter;
  struct tm * startTime;
  
  // We can't do very much here if malloc fails, which is shouldn't since this 
  // is done during system initialization.
  startTime = (struct tm *)malloc(sizeof(struct tm));
  if (startTime == NULL) {
      providers->terminal->debugln("Failed to allocate memory for start time");
  }

  // Make sure we get a proper start time, if we don't we won't be able to sent that info 
  // via the status topic
  while(!getLocalTime(startTime)) {
      providers->terminal->debugln("Failed to get start time, retrying...");
      delay(1000);
  }

  // Start the status tasks loop
  while (true) {

    providers->uuidGenerator->generate();
    double uptime = 0;

    // If we haven't set the start time start time yet, set it.
    struct tm currentTime;
    if (!getLocalTime(&currentTime)) {
        providers->terminal->debugln("Failed to get current time");
    } else {
        // Calculate the uptime
        uptime = mktime(&currentTime) - mktime(startTime);
    }
    
    const char * deviceId  = providers->settingsManager->getSetting(DEVICE_ID_ID);
    const char * statusTopic = providers->settingsManager->getSetting(STATUS_TOPIC_ID);
    MQTTMsg * msg = new MQTTMsg(statusTopic, deviceId, providers->uuidGenerator->toCharArray());
    if (msg == NULL) {
        providers->terminal->debugln("Failed to create msg, there is not enough memory");
        continue;
    }

    msg->setField("uptime", uptime);
    msg->setField("rssi", WiFi.RSSI());
    
    if ( xQueueSend( msgQueue, ( void * ) &msg, MSG_QUEUE_TIMEOUT ) == errQUEUE_FULL) {
        providers->terminal->debugln("Msg queue is full");
        continue;
    }

    providers->terminal->debugln("Queued status");

    // Delay for the status sample rate
    delay(STATUS_SAMPLE_RATE);
  }
}

void PublishMQTTMsg(void * parameter) {
  REMTaskProviders *providers = (REMTaskProviders *)parameter;
  MQTTMsg * msg;

  while (true) {
    // Wait for a message to be received from the queue indefinitely
    // note that this doesn't actually block. 

    if (xQueueReceive(msgQueue, (void *) &msg, portMAX_DELAY) == pdTRUE) {
      if (!providers->pubSubClient->publish(msg->getTopic(), msg->getDocStr())) {
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
    delay(10);
  }
}

void TerminalTask(void *parameter) {
  Terminal *terminal = (Terminal *)parameter;
    
  // Just handle input characters and the yield back to the core
  terminal->handleCharacter();
}