#pragma once

#include <cstdint>
#include <SPI.h>
#include <MFRC522.h>
#include "BaseDevice.h"

class RFIDReader : public BaseDevice
{
  std::uint8_t ssPin;
  std::uint8_t rstPin;
  MFRC522 mfrc522;

public:
  RFIDReader(std::uint8_t ssPin, std::uint8_t rstPin);

  std::shared_ptr<railschema::Event> Loop();
};
