#include "settings_manager.hpp"
#include "terminal.hpp"
#include "EEPROM.h"

SettingsManager::SettingsManager(Terminal * terminal, EEPROMClass * eeprom) {
    this->terminal = terminal;
    this->eeprom = eeprom;
    this->settings = new Settings();
}

SettingsManager::~SettingsManager() {
    // eeprom and terminal are shared so don't delete the 
    // references. Settings is created as a new instance in this class
    delete this->settings;

    // This class initializes the eeprom, if it is destroyed, 
    // we need to close our "file descriptor" 
    EEPROM.end();
}

bool SettingsManager::loadSettings() {

    // Read the data properly from the EEPROM, if we don't get the same amount of bytes
    // as the size that is allocated by ::begin, return an error.
    uint8_t buf[EEPROM_SIZE];
    size_t read_total = this->eeprom->readBytes(STARTING_ADDR, (void *)buf, EEPROM_SIZE - STARTING_ADDR);
    if (read_total != EEPROM_SIZE) {
        this->terminal->debugln("Failed to read from the EEPROM");   
        return false;
    }

    for (int i = 0; i < EEPROM_SIZE - STARTING_ADDR; i++) {
        Serial.printf("Buf[%d]=%c", i, buf[i]);
    }

    this->settings = new Settings();
    this->settings->deserialize(buf, EEPROM_SIZE); 
}

const char * SettingsManager::getWifiPass() {
    return this->settings->password->c_str();
}

const char *  SettingsManager::getWifiSSID() {
    return this->settings->ssid->c_str();
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

bool SettingsManager::setWifiCredentials(char * wifipass, char * wifissid) {

    this->settings->password = new String(wifipass);
    this->settings->ssid = new String(wifissid);

    // Build a buf the size of the eeprom memory alloc
    // and manually clear it.
    uint8_t buf[EEPROM_SIZE];
    memset(buf, 0x00, EEPROM_SIZE);
    
    this->settings->serialize(buf, EEPROM_SIZE);

    if (!EEPROM.writeBytes(0x00, (void *)buf, EEPROM_SIZE)) {
        this->terminal->debugln("Failed to commit changes to EEPROM");
        return false;
    }

    if (!EEPROM.commit()) {
        this->terminal->debugln("Failed to commit changes to EEPROM");
        return false;
    }

    return true;
}