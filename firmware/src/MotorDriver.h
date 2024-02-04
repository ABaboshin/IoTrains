#pragma once

#include <cstdint>

class MotorDriver {
private:
    std::uint8_t pin1;
    std::uint8_t pin2;
public:
    MotorDriver(std::uint8_t pin1, std::uint8_t pin2);

    void Stop() const;
    void Forward(std::uint8_t speed) const;
    void Backward(std::uint8_t speed) const;
};

