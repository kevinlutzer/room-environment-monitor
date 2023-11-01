#include "pm1006.hpp"

PM1006 *pm1006; 

void setup() {
  pm1006 = new PM1006(&Serial1, 4, 5); 
  pm1006->begin();
  Serial.begin(115200);
  Serial.println("Hello World");
}

void loop() {
  delay(2000);
  pm1006->read();
}