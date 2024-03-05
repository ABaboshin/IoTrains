#include <Arduino.h>
#include "Switch.h"

Switch::Switch(const std::string &id, std::uint8_t pin) : BaseDevice(id)
{
  this->pin = pin;
  pinMode(pin, OUTPUT);
}

void Switch::On()
{
  digitalWrite(pin, 1);
}

void Switch::Off()
{
  digitalWrite(pin, 0);
}

std::shared_ptr<railschema::Event> Switch::Loop()
{
  return nullptr;
}

void Switch::DefaultAction()
{
  On();
}

std::shared_ptr<railschema::State> Switch::ProcessCommand(std::shared_ptr<railschema::Command> command)
{
  auto state = std::make_shared<railschema::State>();
  state->id = this->id;

  auto lightCommand = (railschema::LightCommand *)command.get();
  state->command = *lightCommand;

  if (lightCommand->function == railschema::Function::ON) On();
  else Off();

  state->ok = true;

  return state;
}
