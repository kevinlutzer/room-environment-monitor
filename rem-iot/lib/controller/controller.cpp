#include "WiFi.h"
#include "Adafruit_BME280.h"
#include "ArduinoJson.h"
#include "PubSubClient.h"
#include "UUID.h"
#include "Arduino.h"

#include "mqtt_msg.hpp"
#include "settings_manager.hpp"
#include "terminal.hpp"
#include "pin_config.hpp"
#include "controller.hpp" 

REMController::REMController(PM1006K *pm1006k, Adafruit_BME280 *bme280, Terminal * terminal, SettingsManager * settingsManager, QueueHandle_t * msgQueue) {
    this->pm1006k = pm1006k;
    this->bme280 = bme280;
    this->terminal = terminal;
    this->settingsManager = settingsManager;
    this->msgQueue = msgQueue;

    randomSeed(analogRead(A0) || analogRead(A1) || analogRead(A2));
    uint32_t rn = random();

    this->uuidGenerator = new UUID();
    this->uuidGenerator->seed(rn);
}

// bool REMController::publishStatus() {
//     // DynamicJsonDocument json(156);
    
//     // this->uuidGenerator->generate();
//     // json["id"] = this->uuidGenerator->toCharArray();
//     // json["deviceId"] = this->settingsManager->getSetting(DEVICE_ID_ID);    
    
//     // if (this->terminal->isDebug()) {
//     //     this->terminal->debugln("Status to be serialized:");
//     //     serializeJson(json, Serial);
//     // }

//     // const char * statusTopic = this->settingsManager->getSetting(STATUS_TOPIC_ID);
//     // return this->publish(statusTopic, json.as<String>().c_str());
//     return false;
// }

void REMController::queueLatestSensorData() {
    const char * deviceId  = this->settingsManager->getSetting(DEVICE_ID_ID);
    const char * dataTopic = this->settingsManager->getSetting(DATA_TOPIC_ID);

    // Generate a UUID and append it to the data struct
    this->uuidGenerator->generate();
    MQTTMsg * msg = new MQTTMsg(dataTopic, deviceId, this->uuidGenerator->toCharArray());

    // If we can get pm1006 data, add it to the msg
    if (!this->pm1006k->takeMeasurement()) {
        this->terminal->debugln("Failed to take measurement");
    }

    // Apply PM1006K data to the msg
    msg->setField("pm2_5", this->pm1006k->getPM2_5());
    msg->setField("pm1_0", this->pm1006k->getPM1_0());
    msg->setField("pm10", this->pm1006k->getPM10());

    // Apply BMP280 data to the msg
    msg->setField("temperature", this->bme280->readTemperature());
    msg->setField("humidity", this->bme280->readHumidity());
    msg->setField("pressure", this->bme280->readPressure());

    if ( xQueueSend( *this->msgQueue, ( void * ) &msg, MSG_QUEUE_TIMEOUT ) != pdPASS) {
        this->terminal->debugln("Failed to send message to the queue");
    }
}
