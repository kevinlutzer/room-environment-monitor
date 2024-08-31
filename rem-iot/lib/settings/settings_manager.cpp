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
}

bool SettingsManager::loadSettings() {
    // First few bytes of eeprom can't be written to one of my devices for some reason
    // hence we write/read from a starting address > 0x00. 
    size_t read_len = EEPROM_SIZE - STARTING_ADDR;
    uint8_t buf[EEPROM_SIZE];
   
    size_t read_total = this->eeprom->readBytes(STARTING_ADDR, (void *)buf, read_len);
    if (read_total != read_len) {
        this->terminal->debugln("Failed to read from the EEPROM");   
        return false;
    }

    this->settings = new Settings();
    this->settings->deserialize(buf, read_len); 

    return true;
}

const char * SettingsManager::getWifiPass() {
    return this->settings->password;
}

const char * SettingsManager::getWifiSSID() {
    return this->settings->ssid;
}

bool SettingsManager::setWifiCredentials(const char * wifiPass, const char * wifiSSID) {

    strcpy(this->settings->password, wifiPass);
    strcpy(this->settings->ssid, wifiSSID);

    size_t read_len = EEPROM_SIZE - STARTING_ADDR;

    // Build a buf the size of the eeprom memory alloc
    // and manually clear it.
    uint8_t * buf = (uint8_t *)malloc(sizeof(uint8_t) * read_len);
    if (buf == NULL) {
        this->terminal->debugln("Failed to allocate memory for the buffer when setting wifi credentials");
        return false;
    }
    memset(buf, 0x00, read_len);
       
    this->settings->serialize(buf, read_len);

    if (!EEPROM.writeBytes(STARTING_ADDR, (void *)buf, read_len)) {
        this->terminal->debugln("Failed to commit changes to EEPROM");
        return false;
    }

    free(buf);

    if (!EEPROM.commit()) {
        this->terminal->debugln("Failed to commit changes to EEPROM");
        return false;
    }

    return true;
}