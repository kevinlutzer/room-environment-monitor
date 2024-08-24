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
        bool loadSecrets();
        String getWifiPass();
        String getWifiSSID();
        bool setWifiCredentials(String wifipass, String wifissid);
        void print();

    private:
        Terminal * terminalStream;
        EEPROMClass * eeprom;
        Settings * settings;

        void mutateSetting(void (*func)(Settings * s));
};

#endif