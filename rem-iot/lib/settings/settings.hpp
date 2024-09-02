#ifndef _REM_SETTINGS_HPP
#define _REM_SETTINGS_HPP

#include "settings.hpp"
#include "Arduino.h"


// The address that the buffer containing settings data starts at within.
// The datastorage hardware. For w.e reason, I can't seem to write (commit) data 
// to the first few bytes of my prototype's eeprom.
#define EEPROM_START_ADDR 40

// This is the length of the data in a contiguous memory block. 
// Any storage hardware like an epprom should have atleast the amount 
// of space specified by this macro to store the settings properly
#define EEPROM_SIZE 2048

// This is the length of a single setting in characters. 
#define SETTING_LEN 40

// The total amount of settings that are stored in the eeprom. This is used for
// verifying the amount of memory needed for concating all of the settings objects
// into a single buffer.
#define SETTING_QUANTITY 6

// String ids of each of the settings fields
#define PASSWORD_ID "password"
#define SSID_ID "ssid"
#define MQTT_SERVER_ID "mqtt-server"
#define DEVICE_ID_ID "device-id"
#define DATA_TOPIC_ID "data-topic"
#define STATUS_TOPIC_ID "status-topic"

class Settings {

    public:
        Settings();

        char ssid[SETTING_LEN];
        char password[SETTING_LEN];
        char mqtt_server[SETTING_LEN];
        char device_id[SETTING_LEN];
        char data_topic[SETTING_LEN];
        char status_topic[SETTING_LEN];

        void serialize(uint8_t * buf, int len);
        void deserialize(uint8_t * buf, int len);
};

#endif