#include "controller.hpp" 
#include "WiFi.h"
#include "Adafruit_BME280.h"
#include "sntp.h"
#include "ArduinoJson.h"
#include "PubSubClient.h"
#include "../Config.hpp"

#define DEBUG_CONTROLLER 

REMController::REMController(WiFiClass *wifi, PM1006 *pm1006, Adafruit_BME280 *bme280, PubSubClient *pubsubClient) {
    this->pm1006 = pm1006;
    this->bme280 = bme280;
    this->wifi = wifi;
    this->pubsubClient = pubsubClient;

    this->bme280Data = new BME280Data();
    this->pm1006Data = new PM1006Data();
}

bool REMController::publishStatus() {
    DynamicJsonBuffer json(100);
    #ifdef DEBUG_CONTROLLER
        Serial.println("Going to be publishing status");
    #endif
    json["id"] = DEVICE_ID;
    json["description"] = DEVICE_DESCRIPTION;
    
    #ifdef DEBUG_CONTROLLER
        Serial.println("Status to be serialized:");
        serializeJson(json, Serial);
    #endif

    return this->publish(STATUS_TOPIC, json.as<String>().c_str());
}

bool REMController::publishData() {
    #ifdef DEBUG_CONTROLLER
        Serial.println("Going to be publishing data");
    #endif
    DynamicJsonDocument json(1024);
    json["pm25"] = this->pm1006Data->getPM25();
    json["temperature"] = this->bme280Data->getTemperature();
    json["humidity"] = this->bme280Data->getHumidity();
    json["pressure"] = this->bme280Data->getPressure();

    #ifdef DEBUG_CONTROLLER
        Serial.println("Data to be serialized:");
        serializeJson(json, Serial);
    #endif

    return this->publish(DATA_TOPIC, json.as<String>().c_str());\
}

bool REMController::publish(const char * topic, const char * payload) {
    return this->pubsubClient->publish(topic, payload);
}

String REMController::wifiConfig() {
    if (this->wifi->status() != WL_CONNECTED) {
        return "No Connected";
    }

    IPAddress ip = this->wifi->localIP();
    String ipString = ip.toString();

    String res = "IP: ";

    res.concat(ipString);

    return res;
}

bool REMController::refreshPM25() {
    int pm25 = this->pm1006->read();
    if (pm25 == -1) {
        return false;
    }
 
    this->pm1006Data->setPM25(pm25);
    Serial.print("PM25: ");
    Serial.println(pm25);
    return true;
}

bool REMController::refreshBME280() {
    float temp = this->bme280->readTemperature();
    float humidity = this->bme280->readHumidity();
    float pressure = this->bme280->readPressure();

    this->bme280Data->setTemperature(temp);
    this->bme280Data->setHumidity(humidity);
    this->bme280Data->setPressure(pressure);

    return true;
}

bool REMController::setupSNTP() {
    const char* time_zone = "CET-1CEST,M3.5.0,M10.5.0/3";  // TimeZone rule for Europe/Rome including daylight adjustment rules (optional)

    const char* ntpServer1 = "pool.ntp.org";
    const char* ntpServer2 = "time.nist.gov";
    const long  gmtOffset_sec = 3600;
    const int   daylightOffset_sec = 3600;

    // Setup timing config for NTP. This needs to be done before
    // DHCP address is given to the ESP32C3.
    sntp_servermode_dhcp(1);    // (optional)
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);
}

bool REMController::verifyClockSync() {
    // Verify that we have synced the internal clock with an NTP server
    // Short circuit if alter 10 tries we are not synced.
    sntp_sync_status_t sntp_status = sntp_get_sync_status();
    int count = 0;
    while (sntp_status != SNTP_SYNC_STATUS_COMPLETED) {
        delay(500);
        sntp_status = sntp_get_sync_status();
        if (count > 10) {
            return false;
        }
    }

    #ifdef DEBUG_CONTROLLER
        struct tm timeinfo;
        getLocalTime(&timeinfo);
        Serial.print("Current time: ");
        Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    #endif

    return true; 
}

bool REMController::setupWiFi() {

    this->wifi->useStaticBuffers(true);
    bool success = this->wifi->mode(WIFI_STA);
    if (!success) {
        return false;
    }

    const char * wifissid = "lz-guest";

    wl_status_t wifi_status = this->wifi->begin(wifissid, wifipass);

    // Verfiy that we are connected to the WiFi
    int count = 0;
    while (wifi_status != WL_CONNECTED) {
        // Try 10 times and then restart the ESP32C3
        if (count > 10) {
            return false;
        }

        delay(500);
        wifi_status = this->wifi->status();
        count ++;
    }

    #ifdef DEBUG_CONTROLLER
        Serial.println("Connected to WiFi");
    #endif

    return true;
}