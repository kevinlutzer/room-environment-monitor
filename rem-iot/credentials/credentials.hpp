#ifndef _CREDENTIALS_H
#define _CREDENTIALS_H

#include "terminal.hpp"
#include "EEPROM.h"

class Credentials {

    static const int EEPROM_SIZE = 128;
    static const int EEPROM_WIFI_PASS_ADDR = 64;
    static const int EEPROM_WIFI_SSID_ADDR = 1;

    public:
        Credentials(Terminal * debug, EEPROMClass * eeprom);
        bool begin();
        bool loadSecrets();
        String getWifiPass();
        String getWifiSSID();
        bool setCredentials(String wifipass, String wifissid);
        void waitForCommand();

    private:
        Terminal * terminalStream;
        EEPROMClass * eeprom;

        String wifipass;
        String wifissid; 
};

#endif