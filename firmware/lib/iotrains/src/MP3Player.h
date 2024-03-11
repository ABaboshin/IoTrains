#pragma once

#ifndef NO_AUDIO
#include <cstdint>
#include <map>
#include <vector>
#include <string>

#include <AudioTools.h>
#include <AudioCodecs/CodecMP3Helix.h>
#include "BaseDevice.h"

class MP3File
{
public:
  const unsigned char *data;
  long size;
};

// 128kbps
class MP3Player : public BaseDevice
{
  std::shared_ptr<Stream> current;
  std::shared_ptr<AudioSourceCallback> source;
  MP3DecoderHelix decoder;
  std::shared_ptr<AudioPlayer> player;
#ifdef AUDIO_ANALOG
  AnalogAudioStream out;
#endif
#ifdef AUDIO_I2S
  I2SStream out;
#endif
#ifdef AUDIO_PWM
  PWMAudioOutput out;
#endif

  std::map<std::string, MP3File> mp3;

public:
  MP3Player(const std::string &id, int pin, const std::map<std::string, MP3File> &mp3);

  std::shared_ptr<railschema::State> ProcessCommand(std::shared_ptr<railschema::Command> command) override;
  std::shared_ptr<railschema::Event> Loop();

  static Stream *callbackNextStream(int offset);
};
#endif
