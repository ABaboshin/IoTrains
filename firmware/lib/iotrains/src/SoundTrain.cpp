#include <string>
#include <memory>
#include <bits/shared_ptr.h>
#include <Arduino.h>
#include "SoundTrain.h"

SoundTrain::SoundTrain(std::uint8_t fwdPin, std::uint8_t revPin, std::uint8_t fwdLedPin, std::uint8_t revLedPin, std::uint8_t mp3Pin, std::map<std::string, std::vector<unsigned char>> sounds) : Train(fwdPin, revPin, fwdLedPin, revLedPin), mp3(mp3Pin, sounds)
{
}

std::shared_ptr<railschema::State> SoundTrain::ProcessCommand(std::shared_ptr<railschema::Command> command)
{
  return Train::ProcessCommand(command);
}

std::shared_ptr<railschema::Event> SoundTrain::Loop()
{
  mp3.Loop();
  return nullptr;
}
