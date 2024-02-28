#include <Arduino.h>
#include <memory>
#include <ControlUnit.h>
#include "SoundTrain.h"

#include "config.h"

#define STRINGIZE(x) #x
#define STRINGIZE_VALUE_OF(x) STRINGIZE(x)

ControlUnit cu(wifiNetwork, wifiPassword, mqttServer, mqttClientId, mqttLogin, mqttPassword, 1000 * 5);

void setup()
{
  Serial.begin(115200);
  cu.id = STRINGIZE_VALUE_OF(NAME);

  Serial.println("train");
  std::map<std::string, std::vector<unsigned char>> sounds;
  sounds["test"] = std::vector<unsigned char>();
  std::shared_ptr<BaseDevice> train = std::make_shared<SoundTrain>(STRINGIZE_VALUE_OF(NAME), 1, 2, 3, 4, 0, sounds);

  cu.devices.push_back(train);

  cu.Setup();
}

void loop()
{
  cu.Loop();
}
