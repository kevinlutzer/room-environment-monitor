#include "settings.hpp"

Settings::Settings() {
    memset(this->ssid, 0x00, SSID_LEN);
    memset(this->password, 0x00, SSID_LEN);
}

/**
 * serialize takes all of the settings data like the password, ssid, etc and 
 * aligns the values into a uint8_t buffer.
 */
void Settings::serialize(uint8_t * buf, int len) {
    // Build the offset for the pointer arithmatic
    int offset = 0x00;

    // Copy the ssid
    memcpy(buf + offset, this->ssid, SSID_LEN);
    offset += SSID_LEN;

    // Copy the password
    memcpy(buf + offset, this->password, PASSWORD_LEN);
}

void Settings::deserialize(uint8_t * buf, int len) {
    // Build the offset for the pointer arithmatic
    int offset = 0x00;

    // Copy the ssid
    memcpy(this->ssid, buf + offset, SSID_LEN);
    this->ssid[SSID_LEN - 1] = '\0';
    offset += SSID_LEN;

    // Copy the password
    memcpy(this->password, buf + offset, PASSWORD_LEN);
    this->password[PASSWORD_LEN - 1] = '\0';
}