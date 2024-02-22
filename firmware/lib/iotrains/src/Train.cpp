#include <string>
#include <memory>
#include <bits/shared_ptr.h>
#include <Arduino.h>
#include "Train.h"

Train::Train(std::uint8_t fwdPin, std::uint8_t revPin, std::uint8_t fwdLedPin, std::uint8_t revLedPin) : drv(fwdPin, revPin), fwdLed(fwdLedPin), revLed(revLedPin)
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
    drv.Forward(trainCommand->speed);
    fwdLed.On();
    revLed.Off();

    ts->direction = railschema::Direction::FORWARD;
    ts->speed = trainCommand->speed;
    ts->ok = true;
  }

  if (command->function == railschema::Function::MOVE_BACKWARD)
  {
    drv.Backward(trainCommand->speed);
    fwdLed.Off();
    revLed.On();

    ts->direction = railschema::Direction::BACKWARD;
    ts->speed = trainCommand->speed;
    ts->ok = true;
  }

  if (command->function == railschema::Function::BREAK)
  {
    drv.Stop();
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
