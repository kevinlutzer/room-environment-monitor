#include "Arduino.h"
#include "Adafruit_NeoPixel.h"

#include "led_controller.hpp"

LEDController::LEDController(Adafruit_NeoPixel *neoPixel) {
    this->neoPixel = neoPixel;
}

void LEDController::update(double pm1_0, double pm2_5, double pm10) {
    
    Serial.printf("Updating LEDs with PM1.0: %f, PM2.5: %f, PM10: %f\n", pm1_0, pm2_5, pm10);
    
    // Set the colour based on the PM values
    uint32_t colour = 0;
    if (pm1_0 > 12.0 || pm2_5 > 12.0 || pm10 > 12.0) {
        colour = Adafruit_NeoPixel::Color(255, 0, 0);
    } else if (pm1_0 > 6.0 || pm2_5 > 6.0 || pm10 > 6.0) {
        colour = Adafruit_NeoPixel::Color(255, 255, 0);
    } else {
        colour = Adafruit_NeoPixel::Color(0, 255, 0);
    }

    // Set the colour
    for (int i = 0; i < this->neoPixel->numPixels(); i++) {
        this->neoPixel->setPixelColor(i, colour);
    }

    this->neoPixel->show();
}
