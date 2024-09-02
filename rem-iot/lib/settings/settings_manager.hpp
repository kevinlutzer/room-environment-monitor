#ifndef _settingsManager_H
#define _settingsManager_H

#include "terminal.hpp"
#include "settings.hpp"
#include "EEPROM.h"

class SettingsManager {

    public:
        SettingsManager(Terminal * debug, EEPROMClass * eeprom);

        bool loadSettings();
        bool updateSetting(const char * name, const char * value, int value_length);
        const char * getSetting(const char * name);
        bool printSettings(char * buf, size_t len);

    private:
        Terminal * terminal;
        EEPROMClass * eeprom;
        Settings * settings;

        // Used for protecting the settings objects for reads and writes
        SemaphoreHandle_t mutex; 

        

};

#endif