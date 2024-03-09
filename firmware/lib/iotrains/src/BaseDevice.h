#pragma once

#include <vector>
#include <memory>
#include "schema.hpp"

class BaseDevice
{
public:
  std::vector<railschema::Capability> capabilities;
  std::string id;

  BaseDevice(const std::string& id): id(id) {}

  virtual std::shared_ptr<railschema::State> ProcessCommand(std::shared_ptr<railschema::Command> command) = 0;
  virtual std::shared_ptr<railschema::Event> Loop() = 0;
  virtual void DefaultAction() {}
  virtual void Init() {}
};
