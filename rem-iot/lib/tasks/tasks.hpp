#ifndef _TASKS_HPP
#define _TASKS_HPP

#include "PubSubClient.h"
#include "Arduino.h"
#include "controller.hpp"
#include "settings_manager.hpp"
#include "terminal.hpp"

#define SAMPLE_RATE 5000

class REMTaskProviders {

public:
    REMTaskProviders(REMController *controller, SettingsManager *settingsManager, Terminal *terminal, PubSubClient * pubSubClient, QueueHandle_t msgQueue) {
        this->controller = controller;
        this->settingsManager = settingsManager;
        this->terminal = terminal;
        this->pubSubClient = pubSubClient;
        this->msgQueue = msgQueue;
    }

    REMController *controller;
    SettingsManager *settingsManager;
    Terminal *terminal;
    PubSubClient * pubSubClient;
    QueueHandle_t msgQueue;
};


// Task Defs
void PublishDataTask(void *paramater);
void PublishMQTTMsg(void *paramater);
void TerminalTask(void *paramater);

#endif


