#include <Arduino.h>
#include "Switch.h"

Switch::Switch(std::uint8_t pin)
{
  this->pin = pin;
  pinMode(pin, OUTPUT);
}

void Switch::On()
{
  digitalWrite(pin, 1);
}

void Switch::Off()
{
  digitalWrite(pin, 0);
}
