#ifndef _settingsManager_H
#define _settingsManager_H

#include "terminal.hpp"
#include "settings.hpp"
#include "EEPROM.h"

class SettingsManager {

    public:
        SettingsManager(Terminal * debug, EEPROMClass * eeprom);
        ~SettingsManager();
        bool loadSettings();
        const char * getWifiPass();
        const char * getWifiSSID();
        bool setWifiCredentials(const char * wifipass, const char * wifissid);
        void print();

    private:
        Terminal * terminal;
        EEPROMClass * eeprom;
        Settings * settings;
};

#endif