#include <string>
#include <memory>
#include <bits/shared_ptr.h>
#include <Arduino.h>
#include "Train.h"


Train::Train() : drv(5, 6) {
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

    ts->direction = railschema::Direction::FORWARD;
    ts->speed = trainCommand->speed;
    ts->ok = true;
  }

  if (command->function == railschema::Function::MOVE_BACKWARD)
  {
    drv.Backward(trainCommand->speed);
    ts->direction = railschema::Direction::BACKWARD;
    ts->speed = trainCommand->speed;
    ts->ok = true;
  }

  if (command->function == railschema::Function::BREAK)
  {
    drv.Stop();
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
