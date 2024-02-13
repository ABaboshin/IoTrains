#include "MP3Player.h"
#include <Arduino.h>
#include <memory>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#else
#include <WiFi.h>
#include <HTTPClient.h>
#endif

#include <AudioTools.h>
#include <AudioCodecs/CodecMP3Helix.h>

// #include "mp3.h"
static const unsigned char outputmp3[] PROGMEM ={0};

    // data
    std::vector<std::uint8_t> buf;

std::shared_ptr<AudioSourceCallback> source; // source(callbackNextStream);
I2SStream i2s;
MP3DecoderHelix decoder;
std::shared_ptr<AudioPlayer> player; //(source, i2s, decoder);

void callbackPrintMetaData(MetaDataType type, const char *str, int len)
{
  Serial.print("==> ");
  Serial.print(toStr(type));
  Serial.print(": ");
  Serial.println(str);
}

std::shared_ptr<Stream> current;

Stream *callbackNextStream(int offset)
{
  return current.get();
}

MP3Player::MP3Player()
{
  AudioLogger::instance().begin(Serial, AudioLogger::Info);
  // RX

  auto cfg = i2s.defaultConfig(TX_MODE);
  i2s.begin(cfg);
}

std::shared_ptr<railschema::State> MP3Player::ProcessCommand(std::shared_ptr<railschema::Command> command)
{
  auto ts = std::make_shared<railschema::State>();
  ts->id = this->id;
  ts->ok = false;
  auto mp3Command = (railschema::Mp3Command *)command.get();
  ts->command = *mp3Command;
  std::string description;

  if (mp3Command->function == railschema::Function::PLAY_URL)
  {
    HTTPClient http;
    WiFiClient client;
    if (http.begin(client, mp3Command->url.c_str()))
    {
      auto httpCode = http.GET();
      if (httpCode > 0)
      {
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
        {
          auto size = http.getSize();
          buf.resize(size);
          WiFiClient *stream = &client;
          int c = stream->readBytes(&buf[0], size);
          if (c > 0)
          {
            current = std::make_shared<MemoryStream>(&buf[0], size);
            source = std::make_shared<AudioSourceCallback>(callbackNextStream);
            player = std::make_shared<AudioPlayer>(*source, i2s, decoder);
            player->setMetadataCallback(callbackPrintMetaData);
            player->begin();

            ts->ok = true;
          }
        }
      }
    }
  }

  if (mp3Command->function == railschema::Function::PLAY_ID)
  {
    current = std::make_shared<MemoryStream>(outputmp3, sizeof(outputmp3));
    source = std::make_shared<AudioSourceCallback>(callbackNextStream);
    player = std::make_shared<AudioPlayer>(*source, i2s, decoder);
    player->setMetadataCallback(callbackPrintMetaData);
    player->begin();
  }

  if (mp3Command->function == railschema::Function::STOP_PLAY)
  {
    if (player != nullptr)
    {
      player->stop();
      ts->ok = true;
    }
  }

  ts->description = description;

  return ts;
}

std::shared_ptr<railschema::Event> MP3Player::Loop()
{
  if (player != nullptr)
  {
    player->copy();
  }

  return nullptr;
}
