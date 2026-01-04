#ifndef _REM_SETTINGS_HPP
#define _REM_SETTINGS_HPP

#include "Arduino.h"
#include "settings.hpp"

// The address that the buffer containing settings data starts at within.
// The data-storage hardware. For w.e reason, I can't seem to write (commit)
// data to the first few bytes of my prototype's EEPROM.
#define EEPROM_START_ADDR 40

// This is the length of the data in a contiguous memory block.
// Any storage hardware like an EEPROM should have atleast the amount
// of space specified by this macro to store the settings properly
#define EEPROM_SIZE 2048

// First few bytes of EEPROM can't be written to one of my devices for some
// reason hence we write/read from a starting address > 0x00.
#define EEPROM_READ_LEN EEPROM_SIZE - EEPROM_START_ADDR

// This is the length of a single setting in characters.
#define SETTING_LEN 40

// The amount of time to wait for the mutex to be available before returning
#define SETTING_MUTEX_TIMEOUT pdMS_TO_TICKS(10) // 10ms

// The amount of characters needed for each of the settings, their names, and
// some formating character (4)
#define SETTINGS_PRINT_LEN                                                     \
  SETTING_LEN *(SETTING_QUANTITY + 4) + 2 + strlen(SSID_ID) +                  \
      strlen(PASSWORD_ID) + strlen(MQTT_SERVER_ID) + strlen(DEVICE_ID_ID) +    \
      strlen(DATA_TOPIC_ID) + strlen(STATUS_TOPIC_ID)

// The total amount of settings that are stored in the EEPROM. This is used for
// verifying the amount of memory needed for concatenating all of the settings
// objects into a single buffer.
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

  // Initialized status, if false that means that we don't have required information
  // like the SSID, password or MQTT server. We must get that over API first
  bool initialized;

  /**
   * @brief serialize takes all of the settings data like the password, ssid,
   * etc and aligns the values into a uint8_t buffer.
   *
   * @param buf the buffer to write the settings to
   * @param len the length of the buffer
   *
   * @return true if the settings were serialized successfully and false if the
   * settings could not be serialized properly. This is most likely due to the
   * buffer being too small
   *
   */
  void serialize(uint8_t *buf, int len);

  /**
   * @brief deserialize takes a uint8_t buffer and aligns the values into the
   * settings object.
   *
   * @param buf the buffer to read the settings from
   * @param len the length of the buffer
   *
   * @return true if the settings were deserialized successfully and false if
   * the settings could not be deserialized properly. This is most likely due to
   * the buffer being too small
   */
  void deserialize(uint8_t *buf, int len);
};

#endif