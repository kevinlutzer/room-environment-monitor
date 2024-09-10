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

    // We can't do very much here if malloc fails, which is shouldn't since this 
    // is done during system initialization.
    this->startTime = (struct tm *)malloc(sizeof(struct tm));
    if (this->startTime == NULL) {
        this->terminal->debugln("Failed to allocate memory for start time");
    }

    // Make sure we get a proper start time, if we don't we won't be able to sent that info 
    // via the status topic
    while(!getLocalTime(this->startTime)) {
        this->terminal->debugln("Failed to get start time, retrying...");
        delay(1000);
    }
}

REMController::~REMController() {
    free(this->startTime);
    delete this->uuidGenerator;
}

void REMController::queueStatus() {

    this->uuidGenerator->generate();

    double uptime = 0;

    // If we haven't set the start time start time yet, set it.
    struct tm currentTime;
    if (!getLocalTime(&currentTime)) {
        this->terminal->debugln("Failed to get current time");
    } else {
        // Calculate the uptime
        uptime = mktime(&currentTime) - mktime(this->startTime);
    }
    
    const char * deviceId  = this->settingsManager->getSetting(DEVICE_ID_ID);
    const char * statusTopic = this->settingsManager->getSetting(STATUS_TOPIC_ID);
    MQTTMsg * msg = new MQTTMsg(statusTopic, deviceId, this->uuidGenerator->toCharArray());
    if (msg == NULL) {
        this->terminal->debugln("Failed to create msg, there is not enough memory");
        return;
    }

    msg->setField("uptime", uptime);
    msg->setField("rssi", WiFi.RSSI());
    
    if ( xQueueSend( *this->msgQueue, ( void * ) &msg, MSG_QUEUE_TIMEOUT ) == errQUEUE_FULL) {
        this->terminal->debugln("Msg queue is full");
        return;
    }

    this->terminal->debugln("Queued status");
}

void REMController::queueLatestSensorData() {

    this->uuidGenerator->generate();

    const char * deviceId  = this->settingsManager->getSetting(DEVICE_ID_ID);
    const char * dataTopic = this->settingsManager->getSetting(DATA_TOPIC_ID);
    MQTTMsg * msg = new MQTTMsg(dataTopic, deviceId, this->uuidGenerator->toCharArray());   
    if (msg == NULL) {
        this->terminal->debugln("Failed to create msg, there is not enough memory");
        return;
    }

    // If we can get pm1006 data, add it to the msg
    if (!this->pm1006k->takeMeasurement()) {
        this->terminal->debugln("Failed to take measurement");
        // return;
    }

    // Apply PM1006K data to the msg
    msg->setField("pm2_5", this->pm1006k->getPM2_5());
    msg->setField("pm1_0", this->pm1006k->getPM1_0());
    msg->setField("pm10", this->pm1006k->getPM10());

    // Apply BMP280 data to the msg
    msg->setField("temperature", this->bme280->readTemperature());
    msg->setField("humidity", this->bme280->readHumidity());
    msg->setField("pressure", this->bme280->readPressure());

    if ( xQueueSend( *this->msgQueue, ( void * ) &msg, MSG_QUEUE_TIMEOUT ) == errQUEUE_FULL) {
        this->terminal->debugln("Msg queue is full");
        return;
    }

    this->terminal->debugln("Queued latest sensor data");
}
