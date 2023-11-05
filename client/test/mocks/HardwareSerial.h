#pragma once

#ifndef _HARDWARESERIAL_H_
#define _HARDWARESERIAL_H_

#include <stdint.h>
#include "Arduino.h"
#include <stddef.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace {
    class HardwareSerial {
        public:
            HardwareSerial() = default;
            virtual ~HardwareSerial() = default;
            virtual void begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin) = 0;
            virtual uint8_t available(void) = 0;
            virtual void flush(void) = 0;
            virtual size_t write(const uint8_t *buffer, size_t size) = 0;
            virtual int read() = 0;
    };

    class FakeHardwareSerial: public HardwareSerial {
        public:
            FakeHardwareSerial();
            virtual ~FakeHardwareSerial();
            void begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin) override {};
            uint8_t available(void) override {
                if (i < rxbuf_len) {
                    return 1;
                }

                return 0;
            };
            virtual void flush(void) override {};
            virtual size_t write(const uint8_t *buffer, size_t size) override { return -1; };
            virtual int read() override {
                if (i < rxbuf_len) {
                    return rxbuf[i++];
                }

                return 0;
            };

            unsigned char * rxbuf; // place we are in the return buffer
            uint8_t rxbuf_len; // length of the return buffer

        private:
            uint8_t i = 0;
    };


    FakeHardwareSerial::~FakeHardwareSerial() {
        if (rxbuf != NULL) {
            free(rxbuf);
        }
    }

    FakeHardwareSerial::FakeHardwareSerial() : HardwareSerial() {
        rxbuf = NULL;
        rxbuf_len = 0;
    }

    class MockHardwareSerial : public HardwareSerial {
        public:
            MockHardwareSerial(FakeHardwareSerial * _fake);
            MOCK_METHOD(void, begin, (unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin), (override));
            MOCK_METHOD(uint8_t, available, (), (override));
            MOCK_METHOD(void, flush, (), (override));
            MOCK_METHOD(size_t, write, (const uint8_t *buffer, size_t size), (override));
            MOCK_METHOD(int, read, (), (override));

            void DelegateToFake() {
                ON_CALL(*this, read).WillByDefault([this]() {
                    return fake_->read();
                });

                ON_CALL(*this, available).WillByDefault([this]() {
                    return fake_->available();
                });
            }

            FakeHardwareSerial *fake_;
    };

    MockHardwareSerial::MockHardwareSerial(FakeHardwareSerial * _fake): HardwareSerial() {
        this->fake_ = _fake;
    }

}
#endif