#include <HardwareSerial.h>
#include <Arduino.h>
#include <Adafruit_BME280.h>
#include "pm1006.hpp"
#include <stdlib.h>
#include <iostream>
using namespace std;

#define DEBUG_PM1006

/*
* Initializes the UART controller for the PM1006K
*/
void PM1006::begin() {
    this->serial->begin(9600, SERIAL_8N1, this->rx, this->tx);
}

/**
 * Execute a command with the PM1006K. This is a blocking call.
 * @param txbuf the buffer to send to the PM1006K
 * @param txlen the length of the buffer to send to the PM1006K
 * @param rxbuf the buffer to receive from the PM1006K. This doesn't need to be initialized.
 * @param rxlen the length of the buffer to receive data from the PM1006K
 * @return true if the command was successful, false otherwise.
*/
bool PM1006::command(const unsigned char * txbuf, uint8_t txlen, unsigned char * rxbuf, uint8_t rxlen) {
    this->printBuffer("txbuf", txbuf, txlen);

    this->serial->write(txbuf, txlen);

    int i = 0;
    while(this->serial->available()) {
        // short circuit if we end up reading more than 1000 bytes 
        // to avoid blocking a freeRTOS tasks.
        if(i >= 1000) {
           return false;
        }

        rxbuf[i++] = this->serial->read();
        
        if (i == rxlen) {
            break;
        }
    }

    this->printBuffer("rxbuf", rxbuf, rxlen);

    return true;
}

void PM1006::printBuffer(const char * name, const unsigned char * buf, const uint8_t buf_len) {
    #ifdef DEBUG_PM1006
        Serial.print(name);
        Serial.print(": ");
        for (int i = 0; i < buf_len; i++) {
            Serial.print(buf[i], HEX);
            Serial.print(",");
        }

        Serial.println("\n");
    #endif
}

/**
 * @brief Returns the index of the start of the first valid data frame in the buffer.
 * if there is no valid frame, returns -1.
 * @return an integer containing the index of the first valid index in the header.
*/
bool PM1006::isValidFrame(uint8_t * buf, uint8_t buf_len) {
    if (6 > buf_len) {
        return -1;
    }

    return buf[0] == 0x16 && buf[1] == 0x11 && buf[2] == 0x0B;
}

/**
 * Return the PM2.5 value from the PM1006K
 * @return if the there is no valid data, we return -1, otherwise we return a positive integer.
*/
int PM1006::read() {
    uint8_t buf[pm1006_cmd_read_frame_resp_len];

    // Execute command and check if it failed.
    if (!this->command(pm1006_cmd_read_frame, pm1006_cmd_read_frame_len, buf, pm1006_cmd_read_frame_resp_len)) {
        return -1;
    };

    if (this->isValidFrame(buf, pm1006_cmd_read_frame_resp_len)) {
        Serial.println("Valid Frame");
        Serial.write((buf[5] << 8) | buf[6]);
        
        return (buf[5] << 8) | buf[6];
    }

    // Serial.println("Invalid Frame");

    return -1; 
}
