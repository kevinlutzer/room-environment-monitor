#include <gtest/gtest.h>
#include <iostream>
#include "pm1006.hpp"

using namespace std;
using namespace testing;
namespace {
  TEST(PM1006, read_valid_frame) {

    // Has valid header, and valid data at index 5 and 6
    uint8_t buf[20] = {0x16,0x11,0xB,0x00,0x00,0x00,0x0D,0x00,0x00,0x03,0x8F,0x00,0x00,0x00,0x7D,0x02,0x00,0x00,0x0C,0xB1};
    FakeHardwareSerial * _fake;
    _fake->rxbuf = buf;
    _fake->rxbuf_len = 20;

    // Assertions
    MockHardwareSerial *_mock = new MockHardwareSerial(_fake);
    cout << _mock;
    
    _mock->DelegateToFake();
    ON_CALL(*_mock, write).WillByDefault(Return(0));
    EXPECT_CALL(*_mock, write(_, _))
      .With(Args<0,1>(ElementsAre(0x11, 0x02, 0x0b, 0x01, 0xe1)))
      .Times(1);

    PM1006 *pm1006 = new PM1006(_mock, 4, 5);

    int data = pm1006->read();
    
    int expected  = (int)((buf[5] << 8) | buf[6]);
    EXPECT_EQ(expected, data);
  }
}
