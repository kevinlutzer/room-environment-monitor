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
}

unsigned long lastMillis = 0;
void loop()
{

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
  if (millis() - lastMillis > 2000)
  {
    SensorData ss = sa.getSnapshot();
    char* jsonStr = ss.stringify();
    lastMillis = millis();
    publishTelemetry(jsonStr);
    Serial.println("Publish Telemetry");
    //publishTelemetry("Hello World");
  }
}
