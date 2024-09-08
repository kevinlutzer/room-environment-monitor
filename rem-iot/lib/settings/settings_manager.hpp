#ifndef _settingsManager_H
#define _settingsManager_H

#include "terminal.hpp"
#include "settings.hpp"
#include "EEPROM.h"

class SettingsManager {

    public:
        SettingsManager(Terminal * debug, EEPROMClass * eeprom);

        /**
         * @brief Load the settings from the EEPROM into the local settings object
         * 
         * @return true if the settings were loaded successfully and false if the settings
         * could not be loaded properly
         */
        bool loadSettings();

        /**
         * @brief update a specific setting by the name. This command will update the EEPROM
         * if the setting does exist.
         * 
         * @param name the name of the setting to update this must correspond to one of the name
         * ids found in the settings.hpp file
         * @param value the value to update the setting to
         * @param value_length the length of the value. Note that this length must be under the 
         * SETTING_LEN value defined in the settings.hpp file
         */
        bool updateSetting(const char * name, const char * value, int value_length);

        /** 
         * @brief Get a specific setting by the name
         * 
         * @param name the name of the setting to get
         * @return the value of the setting or NULL if the setting does not exist
         */
        const char * getSetting(const char * name);

        /**
         * @brief Format the setting data into a printable format and place the data 
         * into the passed buffer, the length of the buffer must be at least SETTINGS_PRINT_LEN
         * 
         * @param buf the buffer to write the settings to
         * @param len the length of the buffer
         * @return true if the settings were printed successfully and false if the settings
         * could not be printed properly. This is most likely due to the buffer being too small
         */
        bool printSettings(char * buf, size_t len);

    private:
        Terminal * terminal;
        EEPROMClass * eeprom;
        Settings * settings;

        // Used for protecting the settings objects for reads and writes
        SemaphoreHandle_t mutex; 

        

};

#endif