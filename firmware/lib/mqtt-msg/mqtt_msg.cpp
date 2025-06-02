#include "mqtt_msg.hpp"
#include "Arduino.h"
#include "ArduinoJson.h"

MQTTMsg::MQTTMsg(const char *topic, const char *deviceId, const char *id) {

  this->topic[TOPIC_LEN - 1] = '\0';
  this->deviceId[DEVICE_ID_LEN - 1] = '\0';
  this->id[ID_LEN - 1] = '\0';
  
  // Just copy the data so we can keep the MQTTMsg isolated from pointers
  // in the settings manager and settings objects.
  strcpy(this->topic, topic);
  strcpy(this->deviceId, deviceId);
  strcpy(this->id, id);

  // Note that internally within JsonDocument, memory is allocated. We don't
  // need ie to preallocate memory
  this->doc = JsonDocument();

  // Append the id and device id to doc. These pieces of data are shared between
  // status and data messages
  this->doc["id"] = this->id;
  this->doc["deviceId"] = this->deviceId;
}

const char *MQTTMsg::getId() { return this->id; }

const char *MQTTMsg::getTopic() { return this->topic; }

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

void MQTTMsg::setField(const char *field, int32_t value) {
  this->doc[field] = value;
}

size_t MQTTMsg::serialize(char *output) {
  // Serialize the JsonDocument to a char array
  return serializeJson(this->doc, output, MAX_DOC_SIZE);
}
