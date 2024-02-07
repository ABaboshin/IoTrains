#pragma once

#include "schema.hpp"
#include "MotorDriver.h"
#include "BaseDevice.h"

class Train : public BaseDevice
{
private:
    MotorDriver drv;
public:
    Train();
    std::shared_ptr<railschema::State> ProcessCommand(const railschema::Command &command) override;
    std::shared_ptr<railschema::Event> Loop();
};
