#include "settings_manager.hpp"
#include "terminal.hpp"
#include "EEPROM.h"

SettingsManager::SettingsManager(Terminal * terminal, EEPROMClass * eeprom) {
    this->terminal = terminal;
    this->eeprom = eeprom;
    this->settings = new Settings();
    this->mutex = xSemaphoreCreateMutex();
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

bool SettingsManager::updateSetting(const char * name, const char * value, int value_len) {
    // Update the settings object based on the passed name and value
    // Verify the length of the value is not greater than the max length of the setting

    for (int i = 0; i < value_len; i++) {
        Serial.printf("Buf[%d]=%c", i, name[i]);
    }

    if (strncmp(name, "ssid", 4) == 0 && value_len <= SSID_LEN) {
        memcpy(this->settings->ssid, value, SSID_LEN);
    } else if (strncmp(name, "password", 8) == 0 && value_len <= PASSWORD_LEN) {
        memcpy(this->settings->password, value, PASSWORD_LEN);
    } else {        
        return false;
    }

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