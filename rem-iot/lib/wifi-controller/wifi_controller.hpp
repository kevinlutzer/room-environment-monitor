#ifndef WIFI_CONTROLLER_HPP
#define WIFI_CONTROLLER_HPP

#include "WiFi.h"
#include "settings_manager.hpp"
#include "terminal.hpp"

/**
 * @brief WiFiController is a class that is responsible for setting up the WiFi
 * connection and the SNTP connection. Other then in setup, this class's methods
 * are not useful, so it should be deleted soon after use.
 */
class WiFiController {

public:
  WiFiController(WiFiClass *wifi, Terminal *terminal,
                 SettingsManager *settingsManager);

  bool setupWiFi();
  void setupSNTP();
  bool verifyClockSync();

private:
  WiFiClass *wifi;
  Terminal *terminal;
  SettingsManager *settingsManager;
};

#endif