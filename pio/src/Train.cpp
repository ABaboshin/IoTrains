#include <string>
#include <memory>
#include <bits/shared_ptr.h>
#include <Arduino.h>
#include "Train.h"


Train::Train() : drv(10, 11) {
}

std::shared_ptr<railschema::State> Train::ProcessCommand(const railschema::Command &command)
{
  Serial.println("enter process");
  auto ts = std::make_shared<railschema::TrainState>();

  if (command.function == railschema::Function::MOVE_FORWARD)
  {
    const auto speed = stoi(command.value.value());
    drv.Forward(speed);

    ts->direction = railschema::Direction::FORWARD;
    ts->speed = speed;
  }

  if (command.function == railschema::Function::MOVE_BACKWARD)
  {
    const auto speed = stoi(command.value.value());
    drv.Backward(speed);
    ts->direction = railschema::Direction::BACKWARD;
    ts->speed = speed;
  }

  if (command.function == railschema::Function::STOP)
  {
    drv.Stop();
    ts->direction = railschema::Direction::STOP;
    ts->speed = 0;
  }

  Serial.println("exit process");

  return ts;
}


void railschema::TrainState::to_json(json &j)
{
  Serial.println("railschema::TrainState::to_json");
  railschema::to_json(j, *this);
}
