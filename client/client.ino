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
  // pinMode(9, OUTPUT);
  Serial.begin(9600);
  Serial1.begin(115200,SERIAL_8N1, 4,5); //int8_t rxPin=4, int8_t txPin=5
  // Serial.println("\nI2C Scanner");

  // bool ok = Wire.setPins(7, 6);
  // if (!ok) {
  //   Serial.println("Failed to set pins");
  // }
  // Wire.begin();

  // bool status;
  
  // // default settings
  // // (you can also pass in a Wire library object like &Wire2)
  // status = bme.begin();  
  // if (!status) {
  //   Serial.println("Could not find a valid BME280 sensor, check wiring!");
  //   while (1);
  // }
  
  // Serial.println("-- Default Test --");
}

void i2cScan() {
  byte error, address;
  int nDevices;
 
  Serial.println("Scanning...");
 
  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
 
      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
 
  delay(5000);           // wait 5 seconds for next scan
}

void printValues() {
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");
  
  // Convert temperature to Fahrenheit
  /*Serial.print("Temperature = ");
  Serial.print(1.8 * bme.readTemperature() + 32);
  Serial.println(" *F");*/
  
  Serial.print("Pressure = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");
  
  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");
  
  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");
  
  Serial.println();
}

void loop()
{
  // digitalWrite(9, LOW);
  // printValues();
  // delay(2000);

  // digitalWrite(9, HIGH);
  // delay(2000);
  if (Serial1.available()) {     // If anything comes in Serial1 (pins 4 & 5)
    Serial.write(Serial1.read());   // read it and send it out Serial (USB)
  }
}