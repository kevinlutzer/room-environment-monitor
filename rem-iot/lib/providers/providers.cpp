#include "providers.hpp"

REMProviders::REMProviders(SensorAdapter *sensorAdapter, SettingsManager *settingsManager, Terminal *terminal, PubSubClient * pubSubClient, UUID * uuidGenerator, LEDController * ledController) {
    this->sensorAdapter = sensorAdapter;
    this->settingsManager = settingsManager;
    this->terminal = terminal;
    this->pubSubClient = pubSubClient;
    this->uuidGenerator = uuidGenerator;
    this->ledController = ledController;
}