#pragma once

#include <cstdint>

class Switch
{
private:
    std::uint8_t pin;
public:
  Switch(std::uint8_t pin);

  void On();
  void Off();
};
