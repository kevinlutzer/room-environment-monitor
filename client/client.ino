#include <Wire.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C

/*
DATA
Sat Oct 14 16:44:24 2023   b'\x16'
Sat Oct 14 16:44:24 2023   b'\x11'
Sat Oct 14 16:44:24 2023   b'\x0b'
Sat Oct 14 16:44:24 2023   b'\x00'
Sat Oct 14 16:44:24 2023   b'\x00'
Sat Oct 14 16:44:24 2023   b'\x00'
Sat Oct 14 16:44:24 2023   b'\x04'
Sat Oct 14 16:44:24 2023   b'\x00'
Sat Oct 14 16:44:24 2023   b'\x00'
Sat Oct 14 16:44:24 2023   b'\x02'
Sat Oct 14 16:44:24 2023   b'B'
Sat Oct 14 16:44:24 2023   b'\x00'
Sat Oct 14 16:44:24 2023   b'\x00'
Sat Oct 14 16:44:24 2023   b'\x00'
Sat Oct 14 16:44:24 2023   b'x'
Sat Oct 14 16:44:24 2023   b'\x02'
Sat Oct 14 16:44:24 2023   b'\x00'
Sat Oct 14 16:44:24 2023   b'\x00'
Sat Oct 14 16:44:24 2023   b'\x0c'
Sat Oct 14 16:44:24 2023   b'\x00'

SEND:

Sat Oct 14 16:46:13 2023   b'\x11'
Sat Oct 14 16:46:13 2023   b'\x02'
Sat Oct 14 16:46:13 2023   b'\x0b'
Sat Oct 14 16:46:13 2023   b'\x01'
Sat Oct 14 16:46:13 2023   b'\xe1'
*/

void setup()
{
  Serial.begin(9600);
  Serial1.begin(115200,SERIAL_8N1, 4,5, true); //int8_t rxPin=4, int8_t txPin=5

  for (int i=0; i<10; i++) Serial.println(i);

  while(true) {
    Serial.println("Writing and Reading Data: \n");

    uint8_t data[5] = {0x11, 0x02, 0x0B, 0x01, 0xE1};
    Serial1.write(data, 5);

    uint8_t result_len = 100;
    uint8_t result[result_len]; // = (uint8_t * )calloc(result_len, sizeof(result));
    while(!Serial1.available());
    Serial1.readBytes(result, result_len);
    Serial1.flush();
  
    Serial.println("Results: \n");

    for (int i=0; i<result_len; i++) {
      Serial.print(result[i], HEX);
      Serial.println();
    };

    Serial.println("");
  }

}

void loop(){}