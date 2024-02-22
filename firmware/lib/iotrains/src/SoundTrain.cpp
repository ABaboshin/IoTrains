#include <string>
#include <memory>
#include <bits/shared_ptr.h>
#include <Arduino.h>
#include "SoundTrain.h"

SoundTrain::SoundTrain(const std::string &id, std::uint8_t fwdPin, std::uint8_t revPin, std::uint8_t fwdLedPin, std::uint8_t revLedPin, std::uint8_t mp3Pin, const std::map<std::string, std::vector<unsigned char>> &sounds) : Train(id, fwdPin, revPin, fwdLedPin, revLedPin), mp3("built-in", mp3Pin, sounds)
{
  for (auto it = mp3.capabilities.begin(); it != mp3.capabilities.end(); it++) {
    capabilities.push_back(*it);
  }
}

std::shared_ptr<railschema::State> SoundTrain::ProcessCommand(std::shared_ptr<railschema::Command> command)
{
  if (command->discriminator == "Mp3Command") {
    return mp3.ProcessCommand(command);
  }

  return Train::ProcessCommand(command);
}

std::shared_ptr<railschema::Event> SoundTrain::Loop()
{
  return mp3.Loop();
}
