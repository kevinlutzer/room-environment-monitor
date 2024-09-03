

#include "tasks.hpp"
#include "mqtt_msg.hpp"

void PublishDataTask(void *parameter) {
  REMTaskProviders *providers = (REMTaskProviders *)parameter;
  while (true) {
    delay(SAMPLE_RATE);
    providers->controller->queueLatestSensorData();
  }
}

void PublishMQTTMsg(void * parameter) {
  REMTaskProviders *providers = (REMTaskProviders *)parameter;
  MQTTMsg * msg;
  

  while (true) {
    
    Serial.println("Check Queue");

    // Wait for a message to be received from the queue indefinitely
    // note that this doesn't actually block. 
    if (xQueueReceive(providers->msgQueue, &msg, portMAX_DELAY) == pdTRUE) {
      Serial.println("Handling Message");

      if (!providers->pubSubClient->publish("rem/data", msg->getDocStr())) {
        providers->terminal->debugln("Failed to publish message");
      }

      providers->terminal->debugln("Published Message");

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
  REMTaskProviders *providers = (REMTaskProviders *)parameter;
    
  // Just handle input characters and the yield back to the core
  providers->terminal->handleCharacter();
}