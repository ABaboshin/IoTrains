//
// Created by andrey.baboshin on 1/31/2024.
//

#include <Arduino.h>
#include "Led.h"

Led::Led(std::uint8_t pin) {
    this->pin = pin;
    pinMode(pin, OUTPUT);
}

void Led::On() {
    digitalWrite(pin, 1);
}

void Led::Off() {
    digitalWrite(pin, 0);
}
