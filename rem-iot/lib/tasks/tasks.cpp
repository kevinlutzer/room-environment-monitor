

#include "tasks.hpp"
#include "mqtt_msg.hpp"

void QueueDataTask(void *parameter) {
  REMController * controller = (REMController *)parameter;
  while (true) {
    delay(DATA_SAMPLE_RATE);
    controller->queueLatestSensorData();
  }
}

void QueueStatusTask(void *parameter) {
  REMController * controller = (REMController *)parameter;
  while (true) {
    delay(STATUS_SAMPLE_RATE);
    controller->queueStatus ();
  }
}

void PublishMQTTMsg(void * parameter) {
  REMTaskProviders *providers = (REMTaskProviders *)parameter;
  MQTTMsg * msg;

  while (true) {
    // Wait for a message to be received from the queue indefinitely
    // note that this doesn't actually block. 

    if (xQueueReceive(*providers->msgQueue, (void *) &msg, portMAX_DELAY) == pdTRUE) {
      if (!providers->pubSubClient->publish(msg->getTopic(), msg->getDocStr())) {
        providers->terminal->debugln("Failed to publish message");
      } else {
        providers->terminal->debugln("Published Message");
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