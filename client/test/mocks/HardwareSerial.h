#ifndef _HARDWARESERIAL_H_
#define _HARDWARESERIAL_H_

#include <stdint.h>
#include "Arduino.h"
#include <stddef.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

// class RF4463 {
//     public:
//         RF4463() = default;
//         virtual ~RF4463() = default;
//         virtual bool txPacket(uint8_t* buf, int len);
// };

// class MockRF4463: public RF4463 {
//     public:
//         MOCK_METHOD(bool, txPacket, (uint8_t* buf, int len), (override));
//         MOCK_METHOD(void, RF4463, (), ());
// };

class HardwareSerial {
    public:
        HardwareSerial() = default;
        virtual ~HardwareSerial() = default;
        virtual void begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin);
        virtual void flush(void);
        virtual size_t write(const uint8_t *buffer, size_t size);
        virtual size_t read(uint8_t *buffer, size_t size);
};

class MockTurtle : public HardwareSerial {
    public:
        MOCK_METHOD(void, begin, (unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin), (override));
        MOCK_METHOD(void, flush, (), (override));
        MOCK_METHOD(size_t, write, (const uint8_t *buffer, size_t size), (override));
        MOCK_METHOD(size_t, read, (uint8_t *buffer, size_t size), (override));
};

#endif