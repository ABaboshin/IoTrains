#include <Arduino.h>
#include <memory>
#include <ControlUnit.h>
#include "MP3Player.h"
#include "SoundTrain.h"

#include "config.h"

#include "helpers.h"
#include "mp3.h"

ControlUnit cu(wifiNetwork, wifiPassword, mqttServer, mqttClientId, mqttLogin, mqttPassword, 1000 * 5);

void setup()
{
  Serial.begin(115200);
  delay(10);
  cu.id = STRINGIZE_VALUE_OF(NAME);

  Serial.println("train");
  std::map<std::string, MP3File> sounds;
  sounds["test"].data = outputmp3;
  sounds["test"].size = outputmp3_size;
  // = {outputmp3, outputmp3_size}; //(outputmp3, outputmp3 + sizeof(outputmp3));
  std::shared_ptr<BaseDevice> train = std::make_shared<SoundTrain>(STRINGIZE_VALUE_OF(NAME), 1, 2, 3, 4, 0, sounds);
  // std::shared_ptr<BaseDevice> train = std::make_shared<Train>(STRINGIZE_VALUE_OF(NAME), 1, 2, 3, 4);

  cu.devices.push_back(train);

  cu.Setup();

  // digitalWrite(3, 1);
  // digitalWrite(4, 1);
}

void loop()
{
  cu.Loop();
}
