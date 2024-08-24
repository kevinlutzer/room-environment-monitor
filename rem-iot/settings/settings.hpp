#ifndef _REM_SETTINGS_HPP
#define _REM_SETTINGS_HPP

#include "settings.hpp"
#include "Arduino.h"

class Settings {
    public: 
        Settings();
        void serialize(uint8_t * buf, int len);
        void deserialize(uint8_t * buf, int len);
};

#endif