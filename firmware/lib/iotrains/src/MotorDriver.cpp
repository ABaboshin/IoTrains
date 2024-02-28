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

void MotorDriver::Stop() const
{
  analogWrite(pin1, 0);
  analogWrite(pin2, 0);
}

void MotorDriver::Backward(std::uint8_t speed) const
{
  analogWrite(pin1, 0);
  double devider = 255;
  auto s = speed * devider / 100;
  analogWrite(pin2, s);
  Serial.println("pin1");
  Serial.println(pin1);
  Serial.println(0);
  Serial.println("pin2");
  Serial.println(pin2);
  Serial.println(s);
}

void MotorDriver::Forward(std::uint8_t speed) const
{
  double devider = 255;
  auto s = speed * devider / 100;
  analogWrite(pin1, s);
  analogWrite(pin2, 0);

  Serial.println("pin1");
  Serial.println(pin1);
  Serial.println(0);
  Serial.println("pin2");
  Serial.println(pin2);
  Serial.println(s);
}
