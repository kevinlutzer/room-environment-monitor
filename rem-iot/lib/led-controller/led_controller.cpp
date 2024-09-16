#include "Arduino.h"
#include "Adafruit_NeoPixel.h"

#include "led_controller.hpp"

LEDController::LEDController(Adafruit_NeoPixel *neoPixel, Terminal * terminal) {
    this->neoPixel = neoPixel;
    this->terminal = terminal;

    // Used for protecting writes to the LEDs as well as the led enabled variable
    this->mutex = xSemaphoreCreateMutex();
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

void LEDController::update(double pm1_0, double pm2_5, double pm10) {
    
    // Take mutex and return if we get a timeout condition
    if (xSemaphoreTake( this->mutex, LED_MUTEX_TIMEOUT ) != pdTRUE) {
        this->terminal->debugln("Failed to take mutex in LEDController::update");
        return;
    }
    
    // Set the colour based on the PM values
    uint32_t colour = 0;
    if (pm1_0 > 12.0 || pm2_5 > 12.0 || pm10 > 12.0) {
        colour = Adafruit_NeoPixel::Color(0, 0, 252);
    } else if (pm1_0 > 5 || pm2_5 > 6.0 || pm10 > 6.0) {
        colour = Adafruit_NeoPixel::Color(255, 255, 0);
    } else {
        colour = Adafruit_NeoPixel::Color(0, 255, 0);
    }

    // Set the colour
    for (int i = 0; i < this->neoPixel->numPixels(); i++) {
        this->neoPixel->setPixelColor(i, colour);
    }

    this->neoPixel->show();
    xSemaphoreGive( this->mutex );
}
