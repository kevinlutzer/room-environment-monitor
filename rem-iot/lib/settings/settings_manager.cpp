#include "settings_manager.hpp"
#include "terminal.hpp"
#include "EEPROM.h"

SettingsManager::SettingsManager(Terminal * terminalStream, EEPROMClass * eeprom) {
    this->terminalStream = terminalStream;
    this->eeprom = eeprom;
    this->settings = new Settings();
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

// Init the EEPROM, retry 4 times and wait ~1 seconds between each retry
bool SettingsManager::begin() {
    bool success = false;
    int count = 0;

    do {
        success = this->eeprom->begin(EEPROM_SIZE);
        count ++;

        if (!success) {
            delay(1000);
        }

    } while( count < 5 && !success );

    return success;
}

bool SettingsManager::setWifiCredentials(String wifipass, String wifissid) {

    // Declare mutator to just update the password and ssid poritons of the settings
    auto mutator = [](Settings * settings) {
        settings->password = wifipass;
        settings->ssid = wifissid;
    };

    return this->mutateSetting(mutator);
}

bool SettingsManager::mutateSetting(void (*func)(Settings * s)) {

    uint8_t buf[240]
    this->settingsManager->serialize(buf, 240);

    if (!EEPROM.writeBytes(0x00, (void *)buf, 240)) {
        this->terminalStream->debugln("Failed to commit changes to EEPROM");
        return false
    }

    if (!EEPROM.commit()) {
        this->terminalStream->debugln("Failed to commit changes to EEPROM");
        return false
    }

    return true
}