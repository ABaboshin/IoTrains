#include "MotorDriver.h"
#include <Arduino.h>

MotorDriver::MotorDriver(std::uint8_t pin1, std::uint8_t pin2)
{
  this->pin1 = pin1;
  this->pin2 = pin2;

  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  Stop();
}

void MotorDriver::Stop() const {
    analogWrite(pin1, 0);
    analogWrite(pin2, 0);
}

void MotorDriver::Backward(std::uint8_t speed) const {
    analogWrite(pin1, 0);
    analogWrite(pin2, speed * 100 / 255);
}

void MotorDriver::Forward(std::uint8_t speed) const {
    analogWrite(pin1, speed * 100 / 255);
    analogWrite(pin2, 0);
}
