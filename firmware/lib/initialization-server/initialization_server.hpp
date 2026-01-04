#ifdef INITIALIZATION_SERVER_HPP
#define INITIALIZATION_SERVER_HPP

#include "settings.hpp"

/** 
 * Uses 
 */
class InitializationServer {
  public:
    InitializationServer(SettingsManager *settings_manager, WiFiClient *client, Terminal *terminal);

    void begin();
    void handleClient();

  private:
    SettingsManager *settings_manager;
    WiFiClient *client;
    Terminal *terminal
};

#endif