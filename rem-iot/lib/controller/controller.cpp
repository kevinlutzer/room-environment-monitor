#include "WiFi.h"
#include "Adafruit_BME280.h"
#include "ArduinoJson.h"
#include "PubSubClient.h"
#include "UUID.h"

#include "settings_manager.hpp"
#include "terminal.hpp"
#include "pin_config.hpp"
#include "controller.hpp" 

REMController::REMController(WiFiClass *wifi, PM1006K *pm1006k, Adafruit_BME280 *bme280, PubSubClient *pubsubClient, Terminal * terminal, SettingsManager * settingsManager) {
    this->pm1006k = pm1006k;
    this->bme280 = bme280;
    this->wifi = wifi;
    this->pubsubClient = pubsubClient;
    this->terminal = terminal;
    this->settingsManager = settingsManager;

    randomSeed(analogRead(A0) || analogRead(A1) || analogRead(A2));
    uint32_t rn = random();

    this->uuidGenerator = new UUID();
    this->uuidGenerator->seed(rn);
}

bool REMController::publishStatus() {
    DynamicJsonDocument json(156);
    
    this->uuidGenerator->generate();
    json["id"] = this->uuidGenerator->toCharArray();
    json["deviceId"] = this->settingsManager->getSetting(DEVICE_ID_ID);    
    
    if (this->terminal->isDebug()) {
        this->terminal->debugln("Status to be serialized:");
        serializeJson(json, Serial);
    }

    const char * statusTopic = this->settingsManager->getSetting(STATUS_TOPIC_ID);
    return this->publish(statusTopic, json.as<String>().c_str());
}

bool REMController::publishData() {
    this->terminal->debugln("Going to be publishing data");
    const char * deviceId  = this->settingsManager->getSetting(DEVICE_ID_ID);
    DynamicJsonDocument json(1024);
    json["pm2_5"] = this->pm2_5;
    json["pm1_0"] = this->pm1_0;
    json["pm10"] = this->pm10;
    json["deviceId"] = deviceId;

    json["temperature"] = this->temperature;
    json["humidity"] = this->humidity;
    json["pressure"] = this->pressure;

    // Generate a UUID and append it to the data struct
    this->uuidGenerator->generate();
    json["id"] = this->uuidGenerator->toCharArray();

    // Get local time and append it to the data struct
    // struct tm timeinfo;
    // getLocalTime(&timeinfo);
    // char buf[24];
    // sprintf(buf, "%04d-%02d-%02dT%02d:%02d:%02d.000Z", timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

    // json["created"] = buf;

    // if (this->terminal->isDebug()) {
    //     this->terminal->debugln("Data to be serialized:");
    //     serializeJson(json, Serial);        
    // }
    const char * dataTopic = this->settingsManager->getSetting(DATA_TOPIC_ID);
    return this->publish(dataTopic, json.as<String>().c_str());\
}

bool REMController::publish(const char * topic, const char * payload) {
    return this->pubsubClient->publish(topic, payload);
}

bool REMController::refreshPM25() {
    if (!this->pm1006k->takeMeasurement()) {
        this->terminal->debugln("Failed to take measurement");
        return false;
    }
 
    this->pm2_5 = this->pm1006k->getPM2_5();
    this->pm1_0 = this->pm1006k->getPM1_0();
    this->pm10 = this->pm1006k->getPM10();

    return true;
}

bool REMController::refreshBME280() {
    this->temperature = this->bme280->readTemperature();
    this->humidity = this->bme280->readHumidity();
    this->pressure = this->bme280->readPressure();

    return true;
}
