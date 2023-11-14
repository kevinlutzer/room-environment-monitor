#include "credentials.hpp"
#include "EEPROM.h"

Credentials::Credentials(Debug * debugStream) {
    this->debugStream = debugStream;
}

bool Credentials::loadSecrets() {
    this->wifipass = EEPROM.readString(Credentials::EEPROM_WIFI_PASS_ADDR);
    if (this->wifipass == 0) {
        this->debugStream->println("Wifi pass has a length of 0");
        return false;
    }

    this->wifissid = EEPROM.readString(Credentials::EEPROM_WIFI_SSID_ADDR);
    if (this->wifissid == 0) {
        this->debugStream->println("Failed to read wifi ssid from EEPROM");
        return false;
    }
}

String Credentials::getWifiPass() {
    return this->wifipass;
}

String Credentials::getWifiSSID() {
    return this->wifissid;
}

// Init the EEPROM, retry 3 times and wait ~2 seconds between each retry
bool Credentials::begin() {
    bool success = false;
    int retries = 3;

    while (!success && retries > 0) {
        success = EEPROM.begin(64);
        if (success) {
            return success;
        } else {
            retries --;
            this->debugStream->println("Failed to initialise EEPROM");
            delay(2000);
        }
    }

    return success;
}

bool Credentials::setCredentials(String wifipass, String wifissid) {
    this->wifipass = wifipass;
    this->wifissid = wifissid;

    uint8_t wifipassLen = wifipass.length();
    uint8_t wifissidLen = wifissid.length();

    if (EEPROM.writeString(Credentials::EEPROM_WIFI_PASS_ADDR, wifipass) < wifipassLen) {
        this->debugStream->println("Failed to write wifi password to EEPROM");
        return false;
    }

    if (EEPROM.writeString(Credentials::EEPROM_WIFI_SSID_ADDR, wifissid) < wifissidLen) {
        this->debugStream->println("Failed to write wifi ssid to EEPROM");
        return false;
    }

    return true;
}