#include <Arduino.h>
#include <memory>
#include <ControlUnit.h>
#include "SoundTrain.h"

#include "config.h"

ControlUnit cu(wifiNetwork, wifiPassword, mqttServer, mqttClientId, mqttLogin, mqttPassword);

void setup()
{
  Serial.begin(115200);
  cu.id = "basic train";

  std::vector<railschema::Function> functions;

  Serial.println("train");
  std::shared_ptr<BaseDevice> train = std::make_shared<SoundTrain>(1, 2, 3, 4, 0, std::map<std::string, std::vector<unsigned char>>());

  train->id = "train1";
  train->functions = functions;
  train->type = railschema::DeviceType::TRAIN;

  cu.devices.push_back(train);

  cu.Setup();
}

void loop()
{
  cu.Loop();
}
