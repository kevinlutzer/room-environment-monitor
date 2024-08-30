#include "settings.hpp"

Settings::Settings() {
    this->ssid = new String("");
    this->password = new String("");
}

/**
 * serialize takes all of the settings data like the password, ssid, etc and 
 * aligns the values into a uint8_t buffer.
 */
void Settings::serialize(uint8_t * buf, int len) {
    // Build the offset for the pointer arithmatic
    int offset = STARTING_ADDR;

    // Copy the ssid
    const char * ssid_c_str = this->ssid->c_str();
    memcpy(buf + offset, ssid_c_str, this->ssid->length());
    offset += SSID_LEN;

    // Copy the password
    const char * password_c_str = this->password->c_str();
    memcpy(buf + offset, password_c_str, this->password->length());
}

void Settings::deserialize(uint8_t * buf, int len) {
    // Build the offset for the pointer arithmatic
    int offset = 0x00;

    // Copy the ssid
    char ssid[SSID_LEN];
    memcpy(ssid, buf + offset, SSID_LEN);
    offset += SSID_LEN;
    this->ssid = new String(ssid);

    // Copy the password
    char password[PASSWORD_LEN];
    memcpy(password, buf + offset, PASSWORD_LEN);
    this->password = new String(password);
}