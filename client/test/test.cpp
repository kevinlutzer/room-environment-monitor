#include <gtest/gtest.h>
#include <iostream>
#include "pm1006_test.hpp"

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
