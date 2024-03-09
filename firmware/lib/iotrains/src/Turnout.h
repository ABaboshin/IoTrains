#pragma once

#include <cstdint>
#include "BaseDevice.h"
#include "Switch.h"

class Turnout : public BaseDevice
{
private:
  Switch sw1;
  Switch sw2;

public:
  Turnout(const std::string &id, std::uint8_t pin1, std::uint8_t pin2);
  std::shared_ptr<railschema::State> ProcessCommand(std::shared_ptr<railschema::Command> command) override;
  std::shared_ptr<railschema::Event> Loop();
  void Pos1();
  void Pos2();
};
