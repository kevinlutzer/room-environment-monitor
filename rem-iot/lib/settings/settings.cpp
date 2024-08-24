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
    // Copy the ssid
    const char * ssid_c_str = this->ssid->c_str();
    memcpy(buf + STARTING_POS, ssid_c_str, this->ssid->length());

    // Copy the password
    const char * password_c_str = this->password->c_str();
    memcpy(buf + STARTING_POS + SSID_LEN, password_c_str, this->password->length());
}

void Settings::deserialize(uint8_t * buf, int len) {
    
}