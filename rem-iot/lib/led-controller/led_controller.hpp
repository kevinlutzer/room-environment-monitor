#ifndef _LED_CONTROLLER_HPP
#define _LED_CONTROLLER_HPP

#include "Adafruit_NeoPixel.h"
#include "Arduino.h"
#include "pin_config.hpp"
#include "led_controller.hpp"
#include "terminal.hpp"

#define LED_MUTEX_TIMEOUT pdMS_TO_TICKS(10) // 10ms

class LEDController {
    public:
        LEDController(Adafruit_NeoPixel * neoPixel, Terminal * terminal);

        /** 
         * @brief Updates the LEDs (NeoPixels) based on the PM1.0, PM2.5, and PM10 values. The colours are green, yellow, and red
         *  and are computed based on if **any** one of PM_X values are in the. 
         */
        void update(double pm1_0, double pm2_5, double pm10);

        /** 
         * @brief Toggles the enable state of the LEDs. If the LEDs are enabled they are controllable and turned on. If they are disabled
         * they are not controllable and turned off.
         */
        bool toggleEnable();

    private:
        // Providers
        Terminal * terminal;
        Adafruit_NeoPixel * neoPixel;

        // Are the LEDs controllable and turned on at the moment? 
        bool enabled = true;
        // Used for protecting writes to the LEDs as well as the led enabled variable
        SemaphoreHandle_t mutex; 

        // RGB values for the different colours
        uint32_t colour = 0;

        // Private functions for setting LED brightness and colour
        void setBrightness(uint8_t brightness);
        void setColour(uint32_t colour);
};

#endif