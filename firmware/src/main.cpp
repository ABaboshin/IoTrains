#include <Arduino.h>
#include <memory>
#include "ControlUnit.h"
#include "Train.h"
#include "MP3Player.h"
#include "RFIDReader.h"

ControlUnit cu;
RFIDReader* rf;

void setup()
{
  Serial.begin(115200);
  // for (int i = 0; i < 10; i++)
  // {
  //   Serial.println("sleep");
  //   delay(500);
  // }

  Serial.println("start");

  // rf = new RFIDReader(D8, D3);

  cu.id = "cu2";

  // std::vector<railschema::Function> functions;

  // Serial.println("train");
  // std::shared_ptr<BaseDevice> train = std::make_shared<Train>();

  // train->id = "train1";
  // train->functions = functions;
  // train->type = railschema::DeviceType::TRAIN;

  // cu.devices.push_back(train);

  Serial.println("player");
  std::shared_ptr<MP3Player> player = std::make_shared<MP3Player>();
  player->id = "player";
  player->type = railschema::DeviceType::PLAYER;
  cu.devices.push_back(player);

  cu.Setup();
}

void loop()
{
  cu.Loop();
  // rf->Loop();
}
