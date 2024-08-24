#ifndef _REM_PROVIDERS
#define _REM_PROVIDERS

#include "WiFi.h"
#include "PubSubClient.h"
#include "PM1006K.h"
#include "Adafruit_BME280.h"

#include "controller.hpp"
#include "config.hpp"
#include "settings_manager.hpp"
#include "terminal.hpp"

static REMController * controller;
static SettingsManager * settingsManager;
static PM1006K * pm1006k;
static Adafruit_BME280 * bme280;
static PubSubClient * pubsubClient;
static WiFiClient espClient;
static Terminal * terminal;

#endif