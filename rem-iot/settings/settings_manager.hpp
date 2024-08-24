#ifndef _settingsManager_H
#define _settingsManager_H

#include "terminal.hpp"
#include "settings.hpp"
#include "EEPROM.h"

class SettingsManager {

    static const int EEPROM_SIZE = 128;
    static const int EEPROM_WIFI_PASS_ADDR = 64;
    static const int EEPROM_WIFI_SSID_ADDR = 1;

    public:
        SettingsManager(Terminal * debug, EEPROMClass * eeprom);
        bool begin();
        bool loadSecrets();
        String getWifiPass();
        String getWifiSSID();
        bool setsettingsManager(String wifipass, String wifissid);
        void waitForCommand();

    private:
        Terminal * terminalStream;
        EEPROMClass * eeprom;

        String wifipass;
        String wifissid; 

        void mutateSetting(void (*func)(Settings * s));
};

#endif