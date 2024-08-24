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

    for (int i = 0; i < 126; i ++) {
        Serial.printf("Byte[%d] = %c\n",i, buf[i]); 
    }
}

void Settings::deserialize(uint8_t * buf, int len) {
    
}