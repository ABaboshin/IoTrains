//
// Created by andrey.baboshin on 1/31/2024.
//

#pragma once

#include "schema.hpp"
#include "MotorDriver.h"

class Train : public railschema::Device {
private:
    MotorDriver drv;
public:
    Train();
    void ProcessCommand(const railschema::Command& command)const;
};

