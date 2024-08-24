#include "settings_manager.hpp"
#include "terminal.hpp"
#include "EEPROM.h"

SettingsManager::SettingsManager(Terminal * terminalStream, EEPROMClass * eeprom) {
    this->terminalStream = terminalStream;
    this->eeprom = eeprom;
}

bool SettingsManager::loadSecrets() {

    // char * buf;
    // size_t read_total = this->eeprom->readBytes(0x01, (void *)buf, 120);

    // this->wifipass = this->eeprom->readString(SettingsManager::EEPROM_WIFI_PASS_ADDR);
    // if (this->wifipass == 0) {
    //     this->terminalStream->debugln("Wifi pass has a length of 0");
    //     return false;
    // }

    // this->wifissid = this->eeprom->readString(SettingsManager::EEPROM_WIFI_SSID_ADDR);
    // if (this->wifissid == 0) {
    //     this->terminalStream->debugln("Failed to read wifi ssid from EEPROM");
    //     return false;
    // }
}

String SettingsManager::getWifiPass() {
    return this->settings->password;
}

String SettingsManager::getWifiSSID() {
    return this->settings->ssid;
}

// Init the EEPROM, retry 3 times and wait ~1 seconds between each retry
bool SettingsManager::begin() {
    bool success = false;
    int retries = 3;

    while (!this->eeprom->begin(EEPROM_SIZE) && retries < 5) {
        this->terminalStream->debugln("Failed to initialise EEPROM");
        delay(1000);
    }

    return success;
}

bool SettingsManager::setsettingsManager(String wifipass, String wifissid) {

    if (this->eeprom->writeString(SettingsManager::EEPROM_WIFI_PASS_ADDR, wifipass) < wifipass.length()) {
        this->terminalStream->debugln("Failed to write wifi password to EEPROM");
        return false;
    }

    if (this->eeprom->writeString(SettingsManager::EEPROM_WIFI_SSID_ADDR, wifissid) < wifissid.length()) {
        this->terminalStream->debugln("Failed to write wifi ssid to EEPROM");
        return false;
    }

    if (!EEPROM.commit()) {
        this->terminalStream->debugln("Failed to commit changes to EEPROM");
        return false;
    }

    this->wifipass = wifipass;
    this->wifissid = wifissid;

    return true;
}