#include <HardwareSerial.h>
#include <Arduino.h>

#define PM1006_BAUD 9600

class PM1006 {
    public:
        PM1006(HardwareSerial *serial, int8_t rx, int8_t tx);
        ~PM1006();
        void begin();
        void read();

    private:
        HardwareSerial *serial;
        int8_t rx;
        int8_t tx;
};