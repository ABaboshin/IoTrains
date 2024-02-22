#pragma once

#include <cstdint>
#include "BaseDevice.h"
#include "schema.hpp"
#include "MotorDriver.h"
#include "Switch.h"

class Train : public BaseDevice
{
private:
    MotorDriver drv;
    Switch fwdLed;
    Switch revLed;

  public:
    Train(std::uint8_t fwdPin, std::uint8_t revPin, std::uint8_t fwdLedPin, std::uint8_t revLedPin);
    virtual std::shared_ptr<railschema::State> ProcessCommand(std::shared_ptr<railschema::Command> command) override;
    virtual std::shared_ptr<railschema::Event> Loop();
};
