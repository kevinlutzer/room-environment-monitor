#include <string>  

#include "sensor-adapter.h"

using namespace std;

SensorAdapter sa = SensorAdapter();

void setup() {
  Serial.begin(9600);
  
  sa.init();
  uint8_t error = sa.getError();
  if (error > 0) {
    Serial.println("Failed to inialize the sensors with error");
    while(1);
  }
}

void loop() {
  SensorData ss = sa.getSnapshot();
  uint16_t eCO2 = ss.geteCO2();
  uint16_t TVOC = ss.getTVOC();
  float Lux = ss.getLux();
  float Temp = ss.getTemp();
  float Humidity = ss.getHumidity();
  float Pressure = ss.getPressure();
  uint8_t error = sa.getError();

  if(error > 0) {
    Serial.print("Error: ");
    Serial.print(error);
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
  }
  Serial.println("");
  delay(1000);
}
