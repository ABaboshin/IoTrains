#pragma once

#include <cstdint>
#include "BaseDevice.h"

// 128kbps
class MP3Player : public BaseDevice
{
public:
  MP3Player();

  std::shared_ptr<railschema::State> ProcessCommand(const railschema::Command &command) override;
  void Loop();
};
