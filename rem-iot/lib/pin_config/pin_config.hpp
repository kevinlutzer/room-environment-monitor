#ifndef _REM_CONTROLLER_CONFIG_H
#define _REM_CONTROLLER_CONFIG_H

    // #define SAMPLE_RATE 30000 // ms
    #define STATUS_RATE 5000 // ms

    #define MQTT_SERVER "192.168.1.100"
    #define DEVICE_ID "REM-1"
    #define DEVICE_DESCRIPTION "Development unit"
    #define DATA_TOPIC "rem/data"
    #define STATUS_TOPIC "rem/status"

    #define PM1006K_RX_PIN 4
    #define PM1006K_TX_PIN 5

    #define FAN 2

    #define I2C_SDA 7
    #define I2C_SCL 6

#endif