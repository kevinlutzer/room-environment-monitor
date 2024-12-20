#ifndef _LED_CONTROLLER_HPP
#define _LED_CONTROLLER_HPP

#include "Adafruit_NeoPixel.h"
#include "Arduino.h"
#include "led_controller.hpp"
#include "pin_config.hpp"
#include "terminal.hpp"

#define LED_MUTEX_TIMEOUT pdMS_TO_TICKS(10) // 10ms

class LEDController {
public:
  LEDController(Adafruit_NeoPixel *neoPixel, Terminal *terminal);

  /**
   * @brief Clears the LEDs by setting the colour to 0. This is used primarily
   * when we want to shut off the indicator at night and in the early morning
   */
  void clear();

  /**
   * @brief Updates the LEDs (NeoPixels) based on the PM1.0, PM2.5, and PM10
   * values. The colours are green, yellow, and red and are computed based on if
   * **any** one of PM_X values are in the.
   */
  void updateFromPM(double pm1_0, double pm2_5, double pm10);

private:
  // Providers
  Terminal *terminal;
  Adafruit_NeoPixel *neoPixel;

  // Are the LEDs controllable and turned on at the moment?
  bool enabled = true;
  // Used for protecting writes to the LEDs as well as the led enabled variable
  SemaphoreHandle_t mutex;

  // RGB values for the different colours
  uint32_t colour = 0;

  /**
   * @brief Sets the colour of the LEDs to the passed colour. This function will
   * set the colour of all of the LEDs
   * @param colour the colour to set the LEDs to. This should be created by
   * Adafruit_NeoPixel::Color.
   */
  void setColour(uint32_t colour);
};

#endif