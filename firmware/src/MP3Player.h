#pragma once

#include <cstdint>
#include "BaseDevice.h"

// 128kbps
class MP3Player : public BaseDevice
{
public:
  MP3Player();

  std::shared_ptr<railschema::State> ProcessCommand(std::shared_ptr<railschema::Command>  command) override;
  std::shared_ptr<railschema::Event> Loop();
};
