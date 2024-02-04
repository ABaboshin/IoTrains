#include <Arduino.h>
#include <memory>
#include "ControlUnit.h"
#include "Train.h"
#include "MP3Player.h"

ControlUnit cu;

void setup()
{
  Serial.begin(115200);
  for (int i = 0; i < 10; i++)
  {
    Serial.println("sleep");
    delay(500);
  }

  Serial.println("start");

  cu.id = "cu2";

  // std::vector<railschema::Function> functions;
  // functions.push_back(railschema::Function::MOVE_BACKWARD);
  // functions.push_back(railschema::Function::MOVE_FORWARD);
  // functions.push_back(railschema::Function::STOP);

  // std::shared_ptr<BaseDevice> train = std::make_shared<Train>();

  // train->id = "train1";
  // train->functions = functions;
  // train->type = railschema::DeviceType::TRAIN;

  // cu.devices.push_back(train);

  std::shared_ptr<MP3Player> player = std::make_shared<MP3Player>();
  player->id = "player";
  player->type = railschema::DeviceType::PLAYER;
  cu.devices.push_back(player);

  cu.Setup();
}

void loop()
{
  cu.Loop();
}
