#include <string>
#include <memory>
#include <bits/shared_ptr.h>
#include <Arduino.h>
#include "Train.h"
#include <cmath>

inline int calculateSpeed(int speed100)
{
  double devider = 255;
  auto newSpeed = speed100 * devider / 100;
  return (int)std::abs(newSpeed);
}

Train::Train(const std::string &id, std::uint8_t fwdPin, std::uint8_t revPin, std::uint8_t fwdLedPin, std::uint8_t revLedPin) : BaseDevice(id), drv(fwdPin, revPin, 2, 3), fwdLed(fwdLedPin), revLed(revLedPin)
{
  railschema::Capability trainCapability;
  trainCapability.type = railschema::CapabilityType::TRAIN;
  trainCapability.value = "";
  capabilities.push_back(trainCapability);
}

std::shared_ptr<railschema::State> Train::ProcessCommand(std::shared_ptr<railschema::Command> command)
{
  auto ts = std::make_shared<railschema::TrainState>();
  ts->id = this->id;

  auto trainCommand = (railschema::TrainCommand*)command.get();
  ts->command = *trainCommand;

  if (command->function == railschema::Function::MOVE_FORWARD)
  {
    drv.motorGo(calculateSpeed(trainCommand->speed));
    fwdLed.On();
    revLed.Off();

    ts->direction = railschema::Direction::FORWARD;
    ts->speed = trainCommand->speed;
    ts->ok = true;
  }

  if (command->function == railschema::Function::MOVE_BACKWARD)
  {
    drv.motorRev(calculateSpeed(trainCommand->speed));
    fwdLed.Off();
    revLed.On();

    ts->direction = railschema::Direction::BACKWARD;
    ts->speed = trainCommand->speed;
    ts->ok = true;
  }

  if (command->function == railschema::Function::BREAK)
  {
    drv.motorStop();
    fwdLed.Off();
    revLed.Off();

    ts->direction = railschema::Direction::STOP;
    ts->speed = 0;
    ts->ok = true;
  }

  return ts;
}

std::shared_ptr<railschema::Event> Train::Loop()
{
  return nullptr;
}

void Train::DefaultAction() {
  drv.motorGo(50);
  fwdLed.On();
  revLed.Off();
}
