#include <string>
#include <memory>
#include <bits/shared_ptr.h>
#include <Arduino.h>
#include "Train.h"


Train::Train() : drv(5, 6) {
}

std::shared_ptr<railschema::State> Train::ProcessCommand(const railschema::Command &command)
{
  auto ts = std::make_shared<railschema::TrainState>();
  ts->id = this->id;

  if (command.function == railschema::Function::MOVE_FORWARD)
  {
    const auto speed = stoi(command.value.value());
    drv.Forward(speed);

    ts->direction = railschema::Direction::FORWARD;
    ts->speed = speed;
    ts->ok = true;
  }

  if (command.function == railschema::Function::MOVE_BACKWARD)
  {
    const auto speed = stoi(command.value.value());
    drv.Backward(speed);
    ts->direction = railschema::Direction::BACKWARD;
    ts->speed = speed;
    ts->ok = true;
  }

  if (command.function == railschema::Function::STOP)
  {
    drv.Stop();
    ts->direction = railschema::Direction::STOP;
    ts->speed = 0;
    ts->ok = true;
  }

  return ts;
}
