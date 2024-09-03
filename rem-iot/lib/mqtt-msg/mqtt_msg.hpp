#ifndef _MQTT_MSG_HPP
#define _MQTT_MSG_HPP

#define TOPIC_LEN 40

// This should be the same length as the settings len
// use in the Settings class
#define DEVICE_ID_LEN 40

// Expect that the id of a MQTT msg is a 36 characters long
// uuid v4 char string
#define ID_LEN 36 

#include "ArduinoJson.h"
#include "Arduino.h"

// Number of bytes to allocate for the json document
#define JSON_DOC_SIZE 256

class MQTTMsg {
    public:
        MQTTMsg(const char * topic, const char * deviceId, const char * id);
        void serialize(char * buf, int len);

        void setField(const char * field, const char * value);
        void setField(const char * field, int value);
        void setField(const char * field, float value);

        const char * getTopic();
        const char * getDocStr();
    
    private:
        char topic[TOPIC_LEN];
        char deviceId[DEVICE_ID_LEN];
        char id[ID_LEN];

        JsonDocument doc;
};

#endif