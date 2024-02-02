#pragma once

#include <vector>
#include <memory>
#include "BaseDevice.h"

class ControlUnit
{
  void reconnect();
public:
  ControlUnit();
  void Setup();
  void Loop();

  static void callback(char *topic, byte *payload, unsigned int length);

  std::vector<std::shared_ptr<BaseDevice>> devices;
  std::string id;
};
