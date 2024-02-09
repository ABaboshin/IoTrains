#include <Arduino.h>
#include "Turnout.h"

Turnout::Turnout(std::uint8_t pin1, std::uint8_t pin2) : sw1(pin1), sw2(pin2)
{
}

std::shared_ptr<railschema::State> Turnout::ProcessCommand(std::shared_ptr<railschema::Command> command)
{
  auto state = std::make_shared<railschema::State>();
  state->id = this->id;

  auto turnoutCommand = (railschema::Command *)command.get();
  state->command = *turnoutCommand;

  if (command->function == railschema::Function::TURNOUT_POS1)
  {
    sw1.On();
    delay(100);
    sw1.Off();
    state->ok = true;
  }

  if (command->function == railschema::Function::TURNOUT_POS2)
  {
    sw2.On();
    delay(100);
    sw2.Off();
    state->ok = true;
  }

  return state;
}

std::shared_ptr<railschema::Event> Turnout::Loop()
{
  return nullptr;
}
