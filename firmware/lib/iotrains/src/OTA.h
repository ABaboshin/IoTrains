#pragma once

#include <cstdint>
#include "BaseDevice.h"

class OTA : public BaseDevice
{
private:
public:
  OTA(const std::string &id);

  virtual std::shared_ptr<railschema::State> ProcessCommand(std::shared_ptr<railschema::Command> command) override;
  virtual std::shared_ptr<railschema::Event> Loop();
  virtual void DefaultAction() override;
};
