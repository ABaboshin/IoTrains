#pragma once

#include <vector>
#include <memory>
#include "schema.hpp"

class BaseDevice
{
public:
  std::optional<std::vector<railschema::Function>> functions;
  std::string id;
  railschema::DeviceType type;

  virtual std::shared_ptr<railschema::State> ProcessCommand(const railschema::Command &command)=0;
  virtual std::shared_ptr<railschema::Event> Loop() = 0;
};
