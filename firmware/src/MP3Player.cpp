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

#include "AudioFileSourcePROGMEM.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2SNoDAC.h"

AudioGeneratorMP3 mp3;
std::shared_ptr<AudioFileSource> in;
AudioOutputI2SNoDAC out;

// Called when a metadata event occurs (i.e. an ID3 tag, an ICY block, etc.
void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string)
{
  const char *ptr = reinterpret_cast<const char *>(cbData);
  (void)isUnicode; // Punt this ball for now
  // Note that the type and string may be in PROGMEM, so copy them to RAM for printf
  char s1[32], s2[64];
  strncpy_P(s1, type, sizeof(s1));
  s1[sizeof(s1) - 1] = 0;
  strncpy_P(s2, string, sizeof(s2));
  s2[sizeof(s2) - 1] = 0;
  Serial.printf("METADATA(%s) '%s' = '%s'\n", ptr, s1, s2);
  Serial.flush();
}

// Called when there's a warning or error (like a buffer underflow or decode hiccup)
void StatusCallback(void *cbData, int code, const char *string)
{
  const char *ptr = reinterpret_cast<const char *>(cbData);
  // Note that the string may be in PROGMEM, so copy it to RAM for printf
  char s1[64];
  strncpy_P(s1, string, sizeof(s1));
  s1[sizeof(s1) - 1] = 0;
  Serial.printf("STATUS(%s) '%d' = '%s'\n", ptr, code, s1);
  Serial.flush();
}

MP3Player::MP3Player()
{
  audioLogger = &Serial;
  // RX
  mp3.RegisterStatusCB(StatusCallback, (void *)"mp3");
}

std::shared_ptr<railschema::State> MP3Player::ProcessCommand(const railschema::Command &command)
{
  auto ts = std::make_shared<railschema::State>();
  ts->id = this->id;

  if (command.function == railschema::Function::PLAY)
  {
    HTTPClient http;
    WiFiClient client;
    if (http.begin(client, command.value.value().c_str()))
    {
      auto httpCode = http.GET();
      if (httpCode > 0)
      {
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
        {
          auto size = http.getSize();
          std::vector<std::uint8_t> buf(size);
          WiFiClient *stream = &client;
          int c = stream->readBytes(&buf[0], size);
          if (c > 0)
          {
            in = std::make_shared<AudioFileSourcePROGMEM>(&buf[0], buf.size());
            in->RegisterMetadataCB(MDCallback, (void *)"ICY");
            mp3.begin(in.get(), &out);

            ts->ok = true;
          }
        }
      }
    }
  }

  return ts;
}

void MP3Player::Loop()
{
  if (mp3.isRunning())
  {
    if (!mp3.loop())
    {
      mp3.stop();
    }
  }
}
