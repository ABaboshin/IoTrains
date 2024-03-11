#ifndef NO_AUDIO
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

MP3Player *mp3PlayerInstance;

Stream *MP3Player::callbackNextStream(int offset)
{
  // Serial.println("callbackNextStream");
  return mp3PlayerInstance->current.get();
}

MP3Player::MP3Player(const std::string &id, int pin, const std::map<std::string, MP3File> &mp3) : BaseDevice(id), mp3(mp3)
{
  AudioLogger::instance().begin(Serial, AudioLogger::Error);

#ifdef AUDIO_ANALOG
  Serial.println("analog aduio");
  auto cfg = out.defaultConfig();
  out.begin(cfg);
#endif
#ifdef AUDIO_I2S
  Serial.println("i2s aduio");
  auto cfg = out.defaultConfig(TX_MODE);
  if (pin != -1)
  {
    cfg.pin_data = pin;
  }
  out.begin(cfg);
#endif
#ifdef AUDIO_PWM
  Serial.println("pwm aduio");
  auto cfg = out.defaultConfig();
  Pins pwm_pins;
  pwm_pins.push_back(pin);
  out.begin(cfg);
#endif

  mp3PlayerInstance = this;

  railschema::Capability playerCapability;
  playerCapability.type = railschema::CapabilityType::PLAYER;
  playerCapability.value = "";

  railschema::Capability playUrlCapability;
  playUrlCapability.type = railschema::CapabilityType::PLAY_URL;
  playUrlCapability.value = "";

  railschema::Capability stopCapability;
  stopCapability.type = railschema::CapabilityType::STOP_PLAY;
  stopCapability.value = "";

  capabilities.push_back(playerCapability);
  capabilities.push_back(playUrlCapability);
  capabilities.push_back(stopCapability);
  for (auto it = this->mp3.begin(); it != this->mp3.end(); it++)
  {
    railschema::Capability cap;
    cap.type = railschema::CapabilityType::PLAY_ID;
    cap.value = it->first;
    capabilities.push_back(cap);
  }
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
    Serial.println("play url");
    Serial.println(mp3Command->url.c_str());
    HTTPClient http;
    WiFiClient client;
    if (http.begin(client, mp3Command->url.c_str()))
    {
      Serial.println("begin download");
      auto httpCode = http.GET();
      if (httpCode > 0)
      {
        Serial.println("http code > 0");
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
        {
          Serial.println("http ok");
          auto size = http.getSize();
          Serial.println(size);
          buf.resize(size);
          WiFiClient *stream = &client;
          int c = stream->readBytes(&buf[0], size);
          if (c > 0)
          {

            Serial.println("download done");
            if (player != nullptr)
            {
              player->stop();
              current = std::make_shared<MemoryStream>(&buf[0], size);
              player->next();
            }
            else
            {
              current = std::make_shared<MemoryStream>(&buf[0], size);
              source = std::make_shared<AudioSourceCallback>(callbackNextStream);
              player = std::make_shared<AudioPlayer>(*source, out, decoder);
              player->begin();
            }

            ts->ok = true;
          }
          else
          {
            Serial.println("download failed");
          }
        }
        else
        {
          Serial.println("http not ok");
        }
      }
    }
    else
    {
      Serial.println("begin download failed");
    }
  }

  if (mp3Command->function == railschema::Function::PLAY_ID)
  {
    if (mp3.find(mp3Command->url) != mp3.end())
    {
      if (player != nullptr)
      {
        player->stop();
        current = std::make_shared<MemoryStream>(mp3[mp3Command->url].data, mp3[mp3Command->url].size);
        player->next();
      }
      else
      {
        current = std::make_shared<MemoryStream>(mp3[mp3Command->url].data, mp3[mp3Command->url].size);
        source = std::make_shared<AudioSourceCallback>(callbackNextStream);
        player = std::make_shared<AudioPlayer>(*source, out, decoder);
        player->begin();
      }

      ts->ok = true;
    }
    else
    {
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
    // Serial.println("before copy");
    player->copy();
  }

  return nullptr;
}

#endif
