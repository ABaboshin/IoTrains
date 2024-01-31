//
// Created by andrey.baboshin on 1/31/2024.
//

#pragma once

#include <cstdint>

class Led {
private:
    std::uint8_t pin;
public:
    Led(std::uint8_t pin);

    void On();
    void Off();
};

