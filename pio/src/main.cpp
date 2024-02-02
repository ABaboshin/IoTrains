#include <Arduino.h>
#include "ControlUnit.h"
#include "Train.h"

ControlUnit cu;

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 10; i++)
  {
    Serial.println("sleep");
    delay(500);
  }

  Serial.println("start");

  cu.set_id("cu1");

  std::vector<railschema::Device> devices;
  Train t;
  t.set_id("train1");
  devices.push_back(t);

  cu.set_devices(devices);

  cu.Setup();
}

void loop() {
    cu.Loop();
}
