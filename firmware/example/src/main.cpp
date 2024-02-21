#include <Arduino.h>
#include <memory>
#include <ControlUnit.h>
// #include "Train.h"
#include "MP3Player.h"
#include "MP3Player2.h"
// #include "RFIDReader.h"
#include "Train.h"

#include "config.h"

ControlUnit cu(wifiNetwork, wifiPassword, mqttServer, mqttClientId, mqttLogin, mqttPassword);
// RFIDReader* rf;
MP3Player *mp3;
// Train *t;

#include "mp3.h"

void setup()
{
  Serial.begin(115200);
  for (int i = 0; i < 5; i++)
  {
    Serial.println("sleep");
    delay(500);
  }

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

  // Serial.println("player");
  std::map<std::string, std::vector<unsigned char>> data;
  std::string id = "1";
  data[id] = outputmp3;

  auto player = std::make_shared<MP3Player>(
0,
      data
  );
  player->id = "player";
  player->type = railschema::DeviceType::PLAYER;
  cu.devices.push_back(player);
  mp3 = player.get();

  // cu.Setup();

  Serial.println("done start");
}

bool first = true;
void loop()
{
  // Serial.println("loop");
  // delay(500);
  if (first) {
    Serial.println("loop");
    first = false;
    railschema::Mp3Command mp3Command;
    mp3Command.function = railschema::Function::PLAY_ID;
    mp3Command.url = "1";
    Serial.println("loop1");
    std::shared_ptr<railschema::Command> cmd = std::make_shared<railschema::Mp3Command>(mp3Command);
    Serial.println("loop2");
    Serial.println((long long unsigned int)mp3);
    mp3->ProcessCommand(cmd);
    Serial.println("loop3");
    // current = std::make_shared<MemoryStream>(outputmp3, sizeof(outputmp3));
    // source = std::make_shared<AudioSourceCallback>(callbackNextStream);
    // player = std::make_shared<AudioPlayer>(*source, out, decoder);
    // player->setMetadataCallback(callbackPrintMetaData);
    // player->begin();
  } else {
    // player->copy();
    mp3->Loop();
    }
  // cu.Loop();
  // rf->Loop();
}
