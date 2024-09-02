#ifndef WIFI_CONTROLLER_HPP
#define WIFI_CONTROLLER_HPP

#include "WiFi.h"
#include "settings_manager.hpp"
#include "terminal.hpp"

class WiFiController {

    public:
        WiFiController(WiFiClass *wifi, Terminal * terminal, SettingsManager * settingsManager);
        bool setupWiFi();
        void setupSNTP();
        bool verifyClockSync();

    private:
        WiFiClass *wifi;
        Terminal * terminal;
        SettingsManager * settingsManager;
};

#endif