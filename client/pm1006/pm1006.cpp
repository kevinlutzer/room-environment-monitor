#include <HardwareSerial.h>
#include <Arduino.h>
#include "pm1006.hpp"
#include <stdlib.h>

PM1006::PM1006(HardwareSerial *serial, int8_t rx, int8_t tx) {
    this->serial = serial;
    this->rx = rx;
    this->tx = tx;
}

void PM1006::begin() {
    this->serial->begin(9600, SERIAL_8N1, this->rx, this->tx);
}

void PM1006::read() {
    uint8_t buf[5]={0x11, 0x02, 0x0b, 0x01, 0xe1};
    this->serial->write(buf, 5);
    uint8_t readBuf[20]; 
    this->serial->read(readBuf, 20);

    #ifdef DEBUG_PM1006
        for (int i = 0; i < 20; i++) {
            Serial.print(readBuf[i], HEX);
            Serial.print(",");
        }

        Serial.println("\n\n");
    #endif
}