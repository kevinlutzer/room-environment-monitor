#include "settings_manager.hpp"
#include "terminal.hpp"
#include "EEPROM.h"
#include "Arduino.h"

SettingsManager::SettingsManager(Terminal * terminal, EEPROMClass * eeprom) {
    this->terminal = terminal;
    this->eeprom = eeprom;
    this->settings = new Settings();
    this->mutex = xSemaphoreCreateMutex();
}

bool SettingsManager::loadSettings() {
    uint8_t buf[EEPROM_SIZE];
   
    size_t read_total = this->eeprom->readBytes(EEPROM_START_ADDR, (void *)buf, EEPROM_READ_LEN);
    if (read_total != EEPROM_READ_LEN) {
        this->terminal->debugln("Failed to read from the EEPROM");   
        return false;
    }

    this->settings = new Settings();
    this->settings->deserialize(buf, EEPROM_READ_LEN); 

    return true;
}

const char * SettingsManager::getSetting(const char * name) {

    if (strncmp(name, SSID_ID, 4) == 0) {
        return this->settings->ssid;
    } else if (strncmp(name, PASSWORD_ID, 8) == 0) {
        return this->settings->password;
    } else if (strncmp(name, MQTT_SERVER_ID, 10) == 0) {
        return this->settings->mqtt_server;
    } else if (strncmp(name, DEVICE_ID_ID, 9) == 0) {
        return this->settings->device_id;
    } else if (strncmp(name, DATA_TOPIC_ID, 10) == 0) {
        return this->settings->data_topic;
    } else if (strncmp(name, STATUS_TOPIC_ID, 12) == 0) {
        return this->settings->status_topic;
    }

    return NULL;
}

bool SettingsManager::printSettings(char * buf, size_t len) {
    // Calculate the amount of characters needed for each of the settings, their names, and some formating character (4)
    // Like :, ' ', and \r\n for each character as well as before the first string.
    
    if (len < SETTINGS_PRINT_LEN) {
        Serial.printf("Minlen %d, len %d\n", SETTINGS_PRINT_LEN, len);
        return false;
    }

    snprintf(buf, len,
        "\r\n%s: %s\r\n%s: %s\r\n%s: %s\r\n%s: %s\r\n%s: %s\r\n%s: %s\r\n", 
        SSID_ID, 
        this->settings->ssid, 
        PASSWORD_ID,
        this->settings->password,
        MQTT_SERVER_ID,
        this->settings->mqtt_server,
        DEVICE_ID_ID,
        this->settings->device_id,
        DATA_TOPIC_ID,
        this->settings->data_topic,
        STATUS_TOPIC_ID,
        this->settings->status_topic);
    return true;
}

bool SettingsManager::updateSetting(const char * name, const char * value, int value_len) {
    
    // Grab mutex, block multiple writes 
    if( xSemaphoreTake( this->mutex, SETTING_MUTEX_TIMEOUT ) != pdTRUE ) {
        return false;
    }

    // Update the settings object based on the passed name and value
    // Verify the length of the value is not greater than the max length of the setting
    if (strncmp(name, SSID_ID, 4) == 0 && value_len <= SETTING_LEN) {
        memcpy(this->settings->ssid, value, SETTING_LEN);
    } else if (strncmp(name, PASSWORD_ID, 8) == 0 && value_len <= SETTING_LEN) {
        memcpy(this->settings->password, value, SETTING_LEN);
    } else if (strncmp(name, MQTT_SERVER_ID, 10) == 0 && value_len <= SETTING_LEN) {
        memcpy(this->settings->mqtt_server, value, SETTING_LEN);
    } else if (strncmp(name, DEVICE_ID_ID, 9) == 0 && value_len <= SETTING_LEN) {
        memcpy(this->settings->device_id, value, SETTING_LEN);
    } else if (strncmp(name, DATA_TOPIC_ID, 10) == 0 && value_len <= SETTING_LEN) {
        memcpy(this->settings->data_topic, value, SETTING_LEN);
    } else if (strncmp(name, STATUS_TOPIC_ID, 12) == 0 && value_len <= SETTING_LEN) {
        memcpy(this->settings->status_topic, value, SETTING_LEN);
    } else {
        xSemaphoreGive( this->mutex );
        return false;
    }

    // Build a buf the size of the eeprom memory alloc
    // and manually clear it.
    uint8_t * buf = (uint8_t *)malloc(sizeof(uint8_t) * EEPROM_READ_LEN);
    if (buf == NULL) {
        this->terminal->debugln("Failed to allocate memory for the buffer when setting wifi credentials");
        xSemaphoreGive( this->mutex );
        return false;
    }
    memset(buf, 0x00, EEPROM_READ_LEN);

    this->settings->serialize(buf, EEPROM_READ_LEN);

    if (!EEPROM.writeBytes(EEPROM_START_ADDR, (void *)buf, EEPROM_READ_LEN)) {
        this->terminal->debugln("Failed to commit changes to EEPROM");
        xSemaphoreGive( this->mutex );
        return false;
    }

    free(buf);

    if (!EEPROM.commit()) {
        this->terminal->debugln("Failed to commit changes to EEPROM");
        xSemaphoreGive( this->mutex );
        return false;
    }

    xSemaphoreGive( this->mutex );
    return true;
}