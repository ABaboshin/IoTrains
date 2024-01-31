//
// Created by andrey.baboshin on 1/31/2024.
//

#pragma once

#include <vector>
#include "schema.hpp"

class ControlUnit : public railschema::ControlUnit {
    void reconnect();
public:
  ControlUnit();
  void Setup();
  void Loop();

  static void callback(char *topic, byte *payload, unsigned int length);
};

