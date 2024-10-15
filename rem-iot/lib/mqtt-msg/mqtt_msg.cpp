#include "mqtt_msg.hpp"
#include "Arduino.h"
#include "ArduinoJson.h"

MQTTMsg::MQTTMsg(const char *topic, const char *deviceId, const char *id) {

  // Just copy the data so we can keep the MQTTMsg isolated from pointers
  // in the settings manager and settings objects.
  memcpy(this->topic, topic, TOPIC_LEN);
  this->topic[TOPIC_LEN] = '\0';

  memcpy(this->deviceId, deviceId, DEVICE_ID_LEN);
  this->deviceId[DEVICE_ID_LEN] = '\0';

  memcpy(this->id, id, ID_LEN);
  this->id[ID_LEN] = '\0';

  // Note that internally within JsonDocument, memory is allocated. We don't
  // need ie to preallocate memory
  this->doc = JsonDocument();

  // Append the id and device id to doc. These pieces of data are shared between
  // status and data messages
  this->doc["id"] = this->id;
  this->doc["deviceId"] = this->deviceId;
}

const char *MQTTMsg::getTopic() { return this->topic; }

const char *MQTTMsg::getDocStr() { return this->doc.as<String>().c_str(); }

void MQTTMsg::setField(const char *field, const char *value) {
  this->doc[field] = value;
}

void MQTTMsg::setField(const char *field, int value) {
  this->doc[field] = value;
}

void MQTTMsg::setField(const char *field, float value) {
  this->doc[field] = value;
}

void MQTTMsg::setField(const char *field, double value) {
  this->doc[field] = value;
}

void MQTTMsg::setField(const char *field, uint32_t value) {
  this->doc[field] = value;
}
