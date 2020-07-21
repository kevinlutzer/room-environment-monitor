#include <CloudIoTCore.h>

#include "sensor-adapter.h"
#include "esp8266_mqtt.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

SensorAdapter sa = SensorAdapter();

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(250000);
  setupCloudIoT(); // Creates globals for MQTT
  pinMode(LED_BUILTIN, OUTPUT);
  
  sa.init();
  uint8_t error = sa.getError();
  if (error > 0) {
    Serial.print("Failed to initialize the sensors with error: ");
    Serial.print(error);
    Serial.println("");
    while(1) {
      yield();
    }
  }
}

unsigned long lastMillis = 0;
void loop()
{

  SensorData ss = sa.getSnapshot();
  uint16_t eCO2 = ss.geteCO2();
  uint16_t TVOC = ss.getTVOC();
  float Lux = ss.getLux();
  float Temp = ss.getTemp();
  float Humidity = ss.getHumidity();
  float Pressure = ss.getPressure();
  uint8_t error = sa.getError();

  char* jsonStr = ss.stringify();
  Serial.println(jsonStr);

  Serial.println("");
  if(error > 0) {
    Serial.print("Error: ");
    Serial.print(error);
    Serial.println("");
  } else {
    Serial.print(eCO2);
    Serial.print(" ");
    Serial.print(TVOC);
    Serial.print(" ");
    Serial.print(Lux);
    Serial.print(" ");
    Serial.print(Temp);
    Serial.print(" ");
    Serial.print(Humidity);
    Serial.print(" ");
    Serial.print(Pressure);
    Serial.println("");
  }
  Serial.println("");
  
  mqtt->loop();
  delay(10); // <- fixes some issues with WiFi stability
  yield();
 
  if (!mqttClient->connected())
  {
    ESP.wdtDisable();
    connect(); 
    ESP.wdtEnable(0);
  }

  // TODO: Replace with your code here
  if (millis() - lastMillis > 60000)
  {
    lastMillis = millis();
    publishTelemetry(jsonStr);
  }
}
