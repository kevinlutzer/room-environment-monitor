#ifndef _settingsManager_H
#define _settingsManager_H

#include "terminal.hpp"
#include "settings.hpp"
#include "EEPROM.h"

class SettingsManager {

    public:
        SettingsManager(Terminal * debug, EEPROMClass * eeprom);
        bool loadSettings();
        const char * getWifiPass();
        const char * getWifiSSID();
        bool setWifiCredentials(const char * wifipass, const char * wifissid);
        bool updateSetting(const char * name, const char * value, int value_length);


    private:
        Terminal * terminal;
        EEPROMClass * eeprom;
        Settings * settings;

        // Used for protecting the settings objects for reads and writes
        SemaphoreHandle_t mutex; 

        

};

#endif