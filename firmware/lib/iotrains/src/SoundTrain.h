#pragma once
#ifndef NO_AUDIO

#include <cstdint>
#include "Train.h"
#include "MP3Player.h"

class SoundTrain : public Train
{
private:
  MP3Player mp3;

public:
  SoundTrain(const std::string &id, std::uint8_t fwdPin, std::uint8_t revPin, std::uint8_t fwdLedPin, std::uint8_t revLedPin, std::uint8_t mp3Pin, const std::map<std::string, MP3File> &sounds);
  std::shared_ptr<railschema::State> ProcessCommand(std::shared_ptr<railschema::Command> command) override;
  std::shared_ptr<railschema::Event> Loop() override;
};

#endif
