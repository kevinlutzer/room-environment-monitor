#ifndef _CREDENTIALS_H
#define _CREDENTIALS_H

#include "terminal.hpp"

class Credentials {

    static const int EEPROM_SIZE = 64;
    static const int EEPROM_WIFI_PASS_ADDR = 30;
    static const int EEPROM_WIFI_SSID_ADDR = 0;

    public:
        Credentials(Terminal * debug);
        bool begin();
        bool loadSecrets();
        String getWifiPass();
        String getWifiSSID();
        bool setCredentials(String wifipass, String wifissid);
        void waitForCommand();

    private:
        Terminal * terminalStream;
        String wifipass;
        String wifissid; 
};

#endif