#ifndef _REM_SETTINGS_HPP
#define _REM_SETTINGS_HPP

#include "settings.hpp"
#include "Arduino.h"

// This is the length of the data in a contiguous memory block. 
// Any storage hardware like an epprom should have atleast the amount 
// of space specified by this macro to store the settings properly
#define SETTINGS_LEN 128

// The address that the buffer containing settings data starts at within.
// The datastorage hardware. For w.e reason, I can't seem to write (commit) data 
// to the first few bytes of my prototype's eeprom.
#define STARTING_ADDR 40
#define SSID_LEN 40
#define PASSWORD_LEN 40
#define EEPROM_SIZE 2048

class Settings {

    public:
        Settings();

        char ssid[SSID_LEN];
        char password[PASSWORD_LEN];

        void serialize(uint8_t * buf, int len);
        void deserialize(uint8_t * buf, int len);
};

#endif