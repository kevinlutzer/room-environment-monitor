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
    size_t read_len = EEPROM_SIZE - EEPROM_START_ADDR;
    uint8_t buf[EEPROM_SIZE];
   
    size_t read_total = this->eeprom->readBytes(EEPROM_START_ADDR, (void *)buf, read_len);
    if (read_total != read_len) {
        this->terminal->debugln("Failed to read from the EEPROM");   
        return false;
    }

    this->settings = new Settings();
    this->settings->deserialize(buf, read_len); 

    return true;
}

const char * SettingsManager::getSetting(const char * name) {
    if (strncmp(name, SSID_ID, 4) == 0) {
        return this->settings->ssid;
    } else if (strncmp(name, PASSWORD_ID, 8) == 0) {
        return this->settings->password;
    } else {
        return NULL;
    }
}

bool SettingsManager::printSettings(char * buf, size_t len) {
    // Calculate the amount of characters needed for each of the settings, their names, and some formating character (4)
    // Like :, ' ', and \r\n for each character as well as before the first string.
    size_t minLen = SETTING_LEN * (SETTING_QUANTITY + 4) + 2 + strlen(SSID_ID) + strlen(PASSWORD_ID);

    if (len < minLen) {
        Serial.printf("Minlen %d, len %d\n", minLen, len);
        return false;
    }

    snprintf(buf, len, "\r\nssid: %s\r\npassword: %s\r\n", this->settings->ssid, this->settings->password);
    return true;
}

bool SettingsManager::updateSetting(const char * name, const char * value, int value_len) {
    // Update the settings object based on the passed name and value
    // Verify the length of the value is not greater than the max length of the setting
    if (strncmp(name, SSID_ID, 4) == 0 && value_len <= SETTING_LEN) {
        memcpy(this->settings->ssid, value, SETTING_LEN);
    } else if (strncmp(name, PASSWORD_ID, 8) == 0 && value_len <= SETTING_LEN) {
        memcpy(this->settings->password, value, SETTING_LEN);
    } else {        
        return false;
    }

    size_t read_len = EEPROM_SIZE - EEPROM_START_ADDR;

    // Build a buf the size of the eeprom memory alloc
    // and manually clear it.
    uint8_t * buf = (uint8_t *)malloc(sizeof(uint8_t) * read_len);
    if (buf == NULL) {
        this->terminal->debugln("Failed to allocate memory for the buffer when setting wifi credentials");
        return false;
    }
    memset(buf, 0x00, read_len);
       
    this->settings->serialize(buf, read_len);

    if (!EEPROM.writeBytes(EEPROM_START_ADDR, (void *)buf, read_len)) {
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