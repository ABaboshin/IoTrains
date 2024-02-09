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

// #include <StreamLib.h>
#include "mp3.h"

// /**
//  * @brief A simple Stream implementation which is backed by allocated memory.
//  * @ingroup io
//  * @author Phil Schatzmann
//  * @copyright GPLv3
//  *
//  */
// class MemoryStream2 : public AudioStream
// {
// public:
//   /// Constructor for alloction in RAM
//   MemoryStream2(int buffer_size = 512, MemoryType memoryType = RAM)
//   {
//     LOGD("MemoryStream: %d", buffer_size);
//     this->buffer_size = buffer_size;
//     this->memory_type = memoryType;
//     resize(buffer_size);
//   }

//   /// Constructor for data from Progmem, active is set to true automatically by default.
//   MemoryStream2(const uint8_t *buffer, int buffer_size, bool isActive = true, MemoryType memoryType = FLASH_RAM)
//   {
//     LOGD("MemoryStream: %d", buffer_size);
//     setValue(buffer, buffer_size, memoryType);
//     is_active = isActive;
//   }

//   ~MemoryStream2()
//   {
//     TRACED();
//     if (memoryCanChange() && buffer != nullptr)
//       free(buffer);
//   }

//   // resets the read pointer
//   bool begin() override
//   {
//     TRACED();
//     write_pos = memoryCanChange() ? 0 : buffer_size;
//     if (this->buffer == nullptr && memoryCanChange())
//     {
//       resize(buffer_size);
//     }
//     read_pos = 0;
//     is_active = true;
//     return true;
//   }

//   virtual size_t write(uint8_t byte) override
//   {
//     if (!is_active)
//       return 0;
//     if (buffer == nullptr)
//       return 0;
//     int result = 0;
//     if (write_pos < buffer_size)
//     {
//       result = 1;
//       buffer[write_pos] = byte;
//       write_pos++;
//     }
//     return result;
//   }

//   virtual size_t write(const uint8_t *buffer, size_t size) override
//   {
//     if (!is_active)
//       return 0;
//     size_t result = 0;
//     for (size_t j = 0; j < size; j++)
//     {
//       if (!write(buffer[j]))
//       {
//         break;
//       }
//       result = j;
//     }
//     return result;
//   }

//   virtual int available() override
//   {
//     if (!is_active)
//       return 0;
//     if (buffer == nullptr)
//       return 0;
//     int result = write_pos - read_pos;
//     if (result <= 0 && is_loop)
//     {
//       // rewind to start
//       read_pos = rewind_pos;
//       result = write_pos - read_pos;
//       // call callback
//       if (rewind != nullptr)
//         rewind();
//     }
//     return result;
//   }

//   virtual int availableForWrite() override
//   {
//     if (!is_active)
//       return 0;
//     return buffer_size - write_pos;
//   }

//   virtual int read() override
//   {
//     int result = peek();
//     if (result >= 0)
//     {
//       read_pos++;
//     }
//     return result;
//   }

//   virtual size_t readBytes(uint8_t *buffer, size_t length) override
//   {
//     if (!is_active)
//       return 0;
//     size_t count = 0;
//     while (count < length)
//     {
//       int c = read();
//       if (c < 0)
//         break;
//       *buffer++ = (char)c;
//       count++;
//     }
//     return count;
//   }

//   virtual int peek() override
//   {
//     if (!is_active)
//       return -1;
//     int result = -1;
//     if (available() > 0)
//     {
//       result = buffer[read_pos];
//     }
//     return result;
//   }

//   virtual void flush() override {}

//   virtual void end() override
//   {
//     read_pos = 0;
//     is_active = false;
//   }

//   /// clears the audio data: sets all values to 0
//   virtual void clear(bool reset = false)
//   {
//     if (memoryCanChange())
//     {
//       write_pos = 0;
//       read_pos = 0;
//       if (buffer == nullptr)
//       {
//         resize(buffer_size);
//       }
//       if (reset)
//       {
//         // we clear the buffer data
//         memset(buffer, 0, buffer_size);
//       }
//     }
//     else
//     {
//       read_pos = 0;
//       LOGW("data is read only");
//     }
//   }

//   /// Automatically rewinds to the beginning when reaching the end. For wav files we move to pos 44 to ignore the header!
//   virtual void setLoop(bool loop)
//   {
//     is_loop = loop;
//     rewind_pos = 0;
//     if (buffer != nullptr && buffer_size > 12)
//     {
//       if (memcmp("WAVE", buffer + 8, 4) == 0)
//       {
//         rewind_pos = 44;
//       }
//     }
//   }

//   /// Automatically rewinds to the indicated position when reaching the end
//   virtual void setLoop(bool loop, int rewindPos)
//   {
//     is_loop = loop;
//     rewind_pos = rewindPos;
//   }

//   virtual void resize(size_t size)
//   {
//     if (memoryCanChange())
//     {
//       buffer_size = size;
//       switch (memory_type)
//       {
// #if defined(ESP32) && defined(ARDUINO)
//       case PS_RAM:
//         buffer = (buffer == nullptr) ? (uint8_t *)ps_calloc(size, 1) : (uint8_t *)ps_realloc(buffer, size);
//         assert(buffer != nullptr);
//         break;
// #endif
//       default:
//         buffer = (buffer == nullptr) ? (uint8_t *)calloc(size, 1) : (uint8_t *)realloc(buffer, size);
//         assert(buffer != nullptr);
//         break;
//       }
//     }
//   }

//   virtual uint8_t *data()
//   {
//     return buffer;
//   }

//   /// Callback which is executed when we rewind (in loop mode) to the beginning
//   void setRewindCallback(void (*cb)())
//   {
//     this->rewind = cb;
//   }

//   /// Update the values  (buffer and size)
//   void setValue(const uint8_t *buffer, int buffer_size, MemoryType memoryType = FLASH_RAM)
//   {
//     this->buffer_size = buffer_size;
//     this->read_pos = 0;
//     this->write_pos = buffer_size;
//     this->buffer = (uint8_t *)buffer;
//     this->memory_type = memoryType;
//   }

// protected:
//   int write_pos = 0;
//   int read_pos = 0;
//   int buffer_size = 0;
//   int rewind_pos = 0;
//   uint8_t *buffer = nullptr;
//   MemoryType memory_type = RAM;
//   bool is_loop = false;
//   void (*rewind)() = nullptr;
//   bool is_active = false;

//   bool memoryCanChange()
//   {
//     return memory_type != FLASH_RAM;
//   }
// };

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
  // setup output
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

  // if (mp3Command->function == railschema::Function::PLAY_URL)
  // {
  //   HTTPClient http;
  //   WiFiClient client;
  //   if (http.begin(client, mp3Command->url.c_str()))
  //   {
  //     auto httpCode = http.GET();
  //     if (httpCode > 0)
  //     {
  //       if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
  //       {
  //         auto size = http.getSize();
  //         buf.resize(size);
  //         WiFiClient *stream = &client;
  //         int c = stream->readBytes(&buf[0], size);
  //         if (c > 0)
  //         {
  //           current = std::make_shared<MemoryStream>(&buf[0], size);
  //           source = std::make_shared<AudioSourceCallback>(callbackNextStream);
  //           player = std::make_shared<AudioPlayer>(*source, i2s, decoder);
  //           player->setMetadataCallback(callbackPrintMetaData);
  //           player->begin();

  //           ts->ok = true;
  //         }
  //       }
  //     }
  //   }
  // }

  // if (mp3Command->function == railschema::Function::PLAY_ID)
  // {
  //   current = std::make_shared<MemoryStream>(outputmp3, sizeof(outputmp3));
  //   source = std::make_shared<AudioSourceCallback>(callbackNextStream);
  //   player = std::make_shared<AudioPlayer>(*source, i2s, decoder);
  //   player->setMetadataCallback(callbackPrintMetaData);
  //   player->begin();
  // }

  // if (mp3Command->function == railschema::Function::STOP_PLAY)
  // {
  //   if (player != nullptr)
  //   {
  //     player->stop();
  //     ts->ok = true;
  //   }
  // }

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
