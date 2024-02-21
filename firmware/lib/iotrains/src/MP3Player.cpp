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

// data
std::vector<std::uint8_t> buf;

MP3Player* mp3PlayerInstance;

Stream* MP3Player::callbackNextStream(int offset)
{
  return mp3PlayerInstance->current.get();
}

MP3Player::MP3Player(
    int pin,
std::map < std::string, std::vector < unsigned char>> mp3) : mp3(mp3)
{
  AudioLogger::instance().begin(Serial, AudioLogger::Error);

#ifdef AUDIO_ANALOG
  auto cfg = out.defaultConfig();
  out.begin(cfg);
#endif
#ifdef AUDIO_I2S
  auto cfg = out.defaultConfig(TX_MODE);
  if (pin != -1)
  {
    cfg.pin_data = pin;
  }
  out.begin(cfg);
#endif
#ifdef AUDIO_PWM
  auto cfg = out.defaultConfig();
  Pins pwm_pins;
  pwm_pins.push_back(pin);
  out.begin(cfg);
#endif

  mp3PlayerInstance = this;
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
            player = std::make_shared<AudioPlayer>(*source, out, decoder);
            player->begin();

            ts->ok = true;
          }
        }
      }
    }
  }

  if (mp3Command->function == railschema::Function::PLAY_ID)
  {
    if (mp3.find(mp3Command->url) != mp3.end()) {
      current = std::make_shared<MemoryStream>(&mp3[mp3Command->url][0], mp3[mp3Command->url].size());
      source = std::make_shared<AudioSourceCallback>(callbackNextStream);
      player = std::make_shared<AudioPlayer>(*source, out, decoder);
      player->begin();
    } else {
      ts->ok = false;
    }
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
