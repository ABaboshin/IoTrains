#pragma once

#include <cstdint>
#include "BaseDevice.h"

class MP3Player : public BaseDevice
{
private:
    std::uint8_t pin;
public:
  MP3Player(std::uint8_t pin);

  std::shared_ptr<railschema::State> ProcessCommand(const railschema::Command &command) override;
  void loop();
};
