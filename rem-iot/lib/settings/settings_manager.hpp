#ifndef _settingsManager_H
#define _settingsManager_H

#include "terminal.hpp"
#include "settings.hpp"
#include "EEPROM.h"

class SettingsManager {

    static const int EEPROM_SIZE = 2048;

    public:
        SettingsManager(Terminal * debug, EEPROMClass * eeprom);
        bool begin();
        bool loadSettings();
        const char * getWifiPass();
        const char * getWifiSSID();
        bool setWifiCredentials(char * wifipass, char * wifissid);
        void print();

    private:
        Terminal * terminalStream;
        EEPROMClass * eeprom;
        Settings * settings;
};

#endif