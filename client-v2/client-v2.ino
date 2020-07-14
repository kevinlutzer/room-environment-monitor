#include "sensor-adapter.h"
#include <string>  

using namespace std;

SensorAdapter sa;

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
  uint16_t eCO2 = sa.getSnapshot().geteCO2();
  uint16_t TVOC = sa.getSnapshot().getTVOC();
  uint8_t error = sa.getError();
  if(error > 0) {
    Serial.print("Error: ");
    Serial.print(error);
  } else {
    Serial.print(eCO2);
    Serial.print(" ");
    Serial.print(TVOC);
    
  }
  Serial.println("");
  delay(1000);
}
