#include "settings.hpp"

Settings::Settings() {
  memset(this->ssid, 0x00, SETTING_LEN);
  memset(this->password, 0x00, SETTING_LEN);
}

/**
 * serialize takes all of the settings data like the password, ssid, etc and
 * aligns the values into a uint8_t buffer.
 */
void Settings::serialize(uint8_t *buf, int len) {
  // Build the offset for the pointer arithmatic
  int offset = 0x00;

  // Copy the ssid
  memcpy(buf + offset, this->ssid, SETTING_LEN);
  offset += SETTING_LEN;

  // Copy the password
  memcpy(buf + offset, this->password, SETTING_LEN);
  offset += SETTING_LEN;

  // Copy the mqtt server
  memcpy(buf + offset, this->mqtt_server, SETTING_LEN);
  offset += SETTING_LEN;

  // Copy the device id
  memcpy(buf + offset, this->device_id, SETTING_LEN);
  offset += SETTING_LEN;

  // Copy the data topic
  memcpy(buf + offset, this->data_topic, SETTING_LEN);
  offset += SETTING_LEN;

  // Copy the status topic
  memcpy(buf + offset, this->status_topic, SETTING_LEN);
}

void Settings::deserialize(uint8_t *buf, int len) {
  // Build the offset for the pointer arithmatic
  int offset = 0x00;

  // Copy the ssid
  memcpy(this->ssid, buf + offset, SETTING_LEN);
  this->ssid[SETTING_LEN] = '\0';
  offset += SETTING_LEN;

  // Copy the password
  memcpy(this->password, buf + offset, SETTING_LEN);
  this->password[SETTING_LEN] = '\0';
  offset += SETTING_LEN;

  // Copy the mqtt server
  memcpy(this->mqtt_server, buf + offset, SETTING_LEN);
  this->mqtt_server[SETTING_LEN] = '\0';
  offset += SETTING_LEN;

  // Copy the device id
  memcpy(this->device_id, buf + offset, SETTING_LEN);
  this->device_id[SETTING_LEN] = '\0';
  offset += SETTING_LEN;

  // Copy the data topic
  memcpy(this->data_topic, buf + offset, SETTING_LEN);
  this->data_topic[SETTING_LEN] = '\0';
  offset += SETTING_LEN;

  // Copy the status topic
  memcpy(this->status_topic, buf + offset, SETTING_LEN);
  this->status_topic[SETTING_LEN] = '\0';
}