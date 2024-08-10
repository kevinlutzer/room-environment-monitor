#include "WiFi.h"
#include "Adafruit_BME280.h"
#include "sntp.h"
#include "ArduinoJson.h"
#include "PubSubClient.h"
#include "UUID.h"

#include "credentials.hpp"
#include "terminal.hpp"
#include "../config.hpp"
#include "controller.hpp" 

#define DEBUG_CONTROLLER 

REMController::REMController(WiFiClass *wifi, PM1006K *pm1006k, Adafruit_BME280 *bme280, PubSubClient *pubsubClient, Terminal * terminalStream, Credentials * credentials) {
    this->pm1006k = pm1006k;
    this->bme280 = bme280;
    this->wifi = wifi;
    this->pubsubClient = pubsubClient;
    this->terminalStream = terminalStream;
    this->credentials = credentials;

    randomSeed(analogRead(A0) || analogRead(A1) || analogRead(A2));
    uint32_t rn = random();

    this->uuidGenerator = new UUID();
    this->uuidGenerator->seed(rn);
}

bool REMController::publishStatus() {
    DynamicJsonDocument json(156);
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
    json["pm2_5"] = this->pm2_5;
    json["pm1_0"] = this->pm1_0;
    json["pm10"] = this->pm10;
    json["deviceid"] = DEVICE_ID;

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
    if (!this->pm1006k->takeMeasurement()) {
        Serial.println("Failed to take measurement");
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

    String wifissid = this->credentials->getWifiSSID();
    String wifipass = this->credentials->getWifiPass();

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