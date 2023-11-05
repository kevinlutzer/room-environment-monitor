#include <HardwareSerial.h>
#include <Arduino.h>

#ifndef _PM1006_H
#define _PM1006_H

#define PM1006_BAUD 9600

const unsigned char pm1006_cmd_read_frame[5] = {0x11, 0x02, 0x0b, 0x01, 0xe1};
const uint8_t pm1006_cmd_read_frame_len = 5;
const uint8_t pm1006_cmd_read_frame_resp_len = 40;

class PM1006 {
    public:
        PM1006(HardwareSerial *serial, int8_t rx, int8_t tx) {
            this->serial = serial;
            this->rx = rx;
            this->tx = tx;
        };
        ~PM1006();
        void begin();
        int read();

    private:
        HardwareSerial *serial;
        int8_t rx;
        int8_t tx;
        void printBuffer(const char * name, const unsigned char * txbuf, const uint8_t buf_len);
        bool isValidFrame(uint8_t * buf, uint8_t buf_len);
        bool command(const unsigned char * txbuf, uint8_t txlen, unsigned char * rxbuf, uint8_t rxlen);
};

#endif