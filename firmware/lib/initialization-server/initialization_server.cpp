#include "initialization-server.hpp"

void InitializationServer::InitializationServer(SettingsManager *settings_manager, WiFiClient *client, Terminal *terminal) {
    this->settings_manager = settings_manager;
    this->client = client;
    this->terminal = terminal;
}
