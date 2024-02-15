#pragma once

#include <cstdint>
#include <map>
#include <vector>
#include <string>
#include <AudioTools.h>
#include <AudioCodecs/CodecMP3Helix.h>
#include "BaseDevice.h"

// 128kbps
class MP3Player : public BaseDevice
{
  std::shared_ptr<Stream> current;
  std::shared_ptr<AudioSourceCallback> source;
  MP3DecoderHelix decoder;
  std::shared_ptr<AudioPlayer> player;
#ifdef ESPWROOM
  AnalogAudioStream out;
#else
  I2SStream out;
#endif

  std::map<std::string, std::vector<unsigned char>> mp3;

public:
  MP3Player(std::map<std::string, std::vector<unsigned char>> mp3);

  std::shared_ptr<railschema::State> ProcessCommand(std::shared_ptr<railschema::Command>  command) override;
  std::shared_ptr<railschema::Event> Loop();

  static Stream* callbackNextStream(int offset);
};
