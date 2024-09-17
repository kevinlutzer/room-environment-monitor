#include "Arduino.h"
#include "Adafruit_NeoPixel.h"

#include "led_controller.hpp"

LEDController::LEDController(Adafruit_NeoPixel *neoPixel, Terminal * terminal) {
    this->neoPixel = neoPixel;
    this->terminal = terminal;

    // Used for protecting writes to the LEDs as well as the led enabled variable
    this->mutex = xSemaphoreCreateMutex();

    // Intialize the leds by setting the colour to green and setting the brightness on medium
    // Brightness will take effect on the next call to setColour
    this->neoPixel->setBrightness(127);
    this->colour = Adafruit_NeoPixel::Color(0, 255, 0);
    this->setColour(this->colour);
}

bool LEDController::toggleEnable() {
    // Take mutex and return if we get a timeout condition
    if (xSemaphoreTake( this->mutex, LED_MUTEX_TIMEOUT ) != pdTRUE) {
        this->terminal->debugln("Failed to take mutex in LEDController::update");
        return false;
    }

    this->enabled = !this->enabled;

    xSemaphoreGive( this->mutex );

    return this->enabled;
}

void LEDController::setColour(uint32_t colour) {
    // Set the colour and ramp the brightess back up to 255
    for (int i = 0; i < this->neoPixel->numPixels(); i++) {
        this->neoPixel->setPixelColor(i, colour);
        this->neoPixel->show();
    }
}

void LEDController::update(double pm1_0, double pm2_5, double pm10) {
    
    // Take mutex and return if we get a timeout condition
    if (xSemaphoreTake( this->mutex, LED_MUTEX_TIMEOUT ) != pdTRUE) {
        this->terminal->debugln("Failed to take mutex in LEDController::update");
        return;
    }

    if (!this->enabled) {
        xSemaphoreGive( this->mutex );
        return;
    }
    
    // Set the colour based on the PM values
    if (pm1_0 > 12.0 || pm2_5 > 12.0 || pm10 > 12.0) {
        this->colour = Adafruit_NeoPixel::Color(255, 0, 0);
    } else if (pm1_0 > 5 || pm2_5 > 6.0 || pm10 > 6.0) {
        this->colour = Adafruit_NeoPixel::Color(255, 255, 0);
    } else {
        this->colour = Adafruit_NeoPixel::Color(0, 255, 0);
    }

    xSemaphoreGive( this->mutex );

    // Reset to new colour
    this->setColour(this->colour);
}
