#pragma once

#include <cstdint>
#include "BaseDevice.h"
#include "schema.hpp"
#ifndef NO_MOTOR
#include "ESP32MX1508.h"
#endif
#include "Switch.h"

class Train : public BaseDevice
{
private:
#ifndef NO_MOTOR
  MX1508 drv;
#endif
  Switch fwdLed;
  Switch revLed;

public:
  Train(const std::string &id, std::uint8_t fwdPin, std::uint8_t revPin, std::uint8_t fwdLedPin, std::uint8_t revLedPin);
  virtual std::shared_ptr<railschema::State> ProcessCommand(std::shared_ptr<railschema::Command> command) override;
  virtual std::shared_ptr<railschema::Event> Loop();
  virtual void DefaultAction() override;
};
