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
  delay(10);
  cu.id = STRINGIZE_VALUE_OF(NAME);

  Serial.println("train");
  std::map<std::string, std::vector<unsigned char>> sounds;
  sounds["test"] = std::vector<unsigned char>(); //(outputmp3, outputmp3 + sizeof(outputmp3));
  // std::shared_ptr<BaseDevice> train = std::make_shared<SoundTrain>(STRINGIZE_VALUE_OF(NAME), 1, 2, 3, 4, 0, sounds);
  std::shared_ptr<BaseDevice> train = std::make_shared<Train>(STRINGIZE_VALUE_OF(NAME), 1, 2, 3, 4);

  cu.devices.push_back(train);

  cu.Setup();

  // digitalWrite(3, 1);
  // digitalWrite(4, 1);
}

void loop()
{
  cu.Loop();
}
