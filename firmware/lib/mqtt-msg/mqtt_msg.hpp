#ifndef _MQTT_MSG_HPP
#define _MQTT_MSG_HPP

// This is the maximum length that a topic string could be
#define TOPIC_LEN 41

// This should be the same length as the settings len
// use in the Settings class
#define DEVICE_ID_LEN 37

// Expect that the id of a MQTT msg is a 36 characters long
// uuid v4 char string
#define ID_LEN 37

// Max Size of Document. Measured a few messages at 86 bytes.
// If we increase the amount of fields this value should
// be increased
#define MAX_DOC_SIZE 200

#include "Arduino.h"
#include "ArduinoJson.h"

/**
 * @brief MQTTMsg is a class that is used to create a message that can be
 * published to an MQTT broker. The message is considered to be a immutable
 * event, with it's own unique identifier.
 */
class MQTTMsg {
public:
  /**
   * @brief Construct a new MQTTMsg object
   * This constructor makes memcpys of all of the passed paramters instead of
   * storing references.
   *
   * @param topic the topic to publish the message to
   * @param deviceId the device id of the device that is sending the message
   * @param id the id of the message
   */
  MQTTMsg(const char *topic, const char *deviceId, const char *id);

  void setField(const char *field, const char *value);
  void setField(const char *field, int value);
  void setField(const char *field, float value);
  void setField(const char *field, double value);
  void setField(const char *field, uint32_t value);
  void setField(const char *field, int32_t value);

  /**
   * @brief a reference to the topic string that the message will be published
   * to
   */
  const char *getTopic();

  /**
   * @brief a reference to the id of the message
   */
  const char *getId();

  /**
   * @brief Serialize the JsonDocument to a char array
   * 
   * @return size_t the number of bytes written to the output buffer
   */
  size_t serialize(char *output);

private:
  char topic[TOPIC_LEN];
  char deviceId[DEVICE_ID_LEN];
  char id[ID_LEN];

  JsonDocument doc;
};

#endif