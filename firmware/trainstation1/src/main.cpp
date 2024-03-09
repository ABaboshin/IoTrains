#include <Arduino.h>
#include <memory>
#include <ControlUnit.h>
#include "RFIDReader.h"
#include "helpers.h"

#include "config.h"

ControlUnit cu(wifiNetwork, wifiPassword, mqttServer, mqttClientId, mqttLogin, mqttPassword, 1000 * 5);

void setup()
{
  Serial.begin(115200);
  delay(10);
  cu.id = STRINGIZE_VALUE_OF(NAME);

  std::shared_ptr<BaseDevice> rfid2 = std::make_shared<RFIDReader>("rfid2", 5, 22);
  cu.devices.push_back(rfid2);
  std::shared_ptr<BaseDevice> rfid1 = std::make_shared<RFIDReader>("rfid1", 32, 22);
  cu.devices.push_back(rfid1);

  cu.Setup();
}

void loop()
{
  cu.Loop();
}
