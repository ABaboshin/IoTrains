#include <Arduino.h>
#include <memory>
#include <ControlUnit.h>
#include "Train.h"

#include "config.h"

#include "helpers.h"
// #include "mp3.h"
#include "ESP32MX1508.h"
#include "Tlc5940.h"

ControlUnit cu(wifiNetwork, wifiPassword, mqttServer, mqttClientId, mqttLogin, mqttPassword, 1000 * 5);

void setup()
{
  Serial.begin(115200);
  delay(100);
  cu.id = STRINGIZE_VALUE_OF(NAME);

  for (int i = 0; i < 5; i++)
  {
  Serial.println("train");
    delay(1000);
  }

  Serial.println("init tlc5940");
  delay(1000);
  Tlc5940 *tlc = new Tlc5940();
  tlc->init();
  Serial.println("init tlc5940 done");
  delay(1000);

for (int i = 0; i < 10000; i++)
  {
  Serial.println("led off");
    for (int j = 0; j < 16; j++)
    {
      tlc->set(j, 0);
    }
    tlc->update();
    delay(10000);
  Serial.println("led on");
    for (int j = 0; j < 16; j++)
    {
      tlc->set(j, 4095);
    }
    tlc->update();
    delay(10000);
  }

  /*

  esp32 - tlc5940
  11 - gsclk 2
  12 - blank 3
  13 - sin 6
  14 - sclk 5
  15 - xlat 4
  16 - vrpg 7
  */

  // MX1508 drv = MX1508(11, 12, 2, 3);

  // Serial.println("stop");
  // drv.motorBrake();
  // delay(5000);

  // Serial.println("go");
  // drv.motorGo(255);
  // delay(5000);

  // Serial.println("reverse");
  // drv.motorRev(255);
  // delay(5000);

  // std::map<std::string, MP3File> sounds;
  // sounds["test"].data = outputmp3;
  // sounds["test"].size = outputmp3_size;
  // = {outputmp3, outputmp3_size}; //(outputmp3, outputmp3 + sizeof(outputmp3));
  // std::shared_ptr<BaseDevice> train = std::make_shared<SoundTrain>(STRINGIZE_VALUE_OF(NAME), 1, 2, 3, 4, 0, sounds);

  // std::shared_ptr<BaseDevice> train = std::make_shared<Train>(STRINGIZE_VALUE_OF(NAME), 11, 12, 1, 2);

  // cu.devices.push_back(train);

  // cu.Setup();
}

void loop()
{
  cu.Loop();
}
