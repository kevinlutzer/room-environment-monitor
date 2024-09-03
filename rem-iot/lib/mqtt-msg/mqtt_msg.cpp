#include "ArduinoJson.h"
#include "Arduino.h"
#include "mqtt_msg.hpp" 

MQTTMsg::MQTTMsg(const char * topic, const char * deviceId, const char * id) {

    // Just copy the data so we can keep the MQTTMsg isolated from pointers 
    // in the settings manager and settings objects.
    memcpy(this->topic, topic, TOPIC_LEN);
    memcpy(this->deviceId, deviceId, DEVICE_ID_LEN);
    memcpy(this->id, id, ID_LEN);

    // Note that internally within JsonDocument, memory is allocated. We don't need
    // ie to preallocate memory
    this->doc = JsonDocument();
}

const char * MQTTMsg::getTopic() {
    return this->topic;
}

const char * MQTTMsg::getDocStr() {
    return this->doc.as<String>().c_str();
}

void MQTTMsg::setField(const char * field, const char * value) {
    this->doc[field] = value;
}

void MQTTMsg::setField(const char * field, int value) {
    this->doc[field] = value;
}

void MQTTMsg::setField(const char * field, float value) {
    this->doc[field] = value;
}
