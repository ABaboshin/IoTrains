#pragma once

#include <cstdint>
#include "BaseDevice.h"

class Switch : public BaseDevice
{
private:
    std::uint8_t pin;
public:
  Switch(const std::string &id, std::uint8_t pin);

  void On();
  void Off();

  virtual std::shared_ptr<railschema::State> ProcessCommand(std::shared_ptr<railschema::Command> command) override;
  virtual std::shared_ptr<railschema::Event> Loop();
  virtual void DefaultAction() override;
};
