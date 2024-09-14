#ifndef _LED_CONTROLLER_HPP
#define _LED_CONTROLLER_HPP

#include "Adafruit_NeoPixel.h"
#include "Arduino.h"
#include "pin_config.hpp"
#include "led_controller.hpp"

class LEDController {
    public:
        LEDController(Adafruit_NeoPixel * neoPixel);

        /** 
         * @brief Updates the LEDs (NeoPixels) based on the PM1.0, PM2.5, and PM10 values. The colours are green, yellow, and red
         *  and are computed based on if **any** one of PM_X values are in the. 
         */
        void update(double pm1_0, double pm2_5, double pm10);

    private:
        Adafruit_NeoPixel * neoPixel;
};

#endif