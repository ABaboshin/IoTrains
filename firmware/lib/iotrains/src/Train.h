#pragma once

#include <cstdint>
#include "BaseDevice.h"
#include "schema.hpp"
#include "ESP32MX1508.h"
#include "Switch.h"

class Train : public BaseDevice
{
private:
  MX1508 drv;
  Switch fwdLed;
  Switch revLed;

public:
  Train(const std::string &id, std::uint8_t fwdPin, std::uint8_t revPin, std::uint8_t fwdLedPin, std::uint8_t revLedPin);
  virtual std::shared_ptr<railschema::State> ProcessCommand(std::shared_ptr<railschema::Command> command) override;
  virtual std::shared_ptr<railschema::Event> Loop();
  virtual void DefaultAction() override;
};
