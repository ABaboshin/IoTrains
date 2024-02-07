#include "RFIDReader.h"
#include <Arduino.h>
#include <memory>

/*
sda d8
sck d5
mosi d7
miso d6
irq -
gnd gnd
rst d3
3.3 3.3

*/

RFIDReader::RFIDReader(std::uint8_t ssPin, std::uint8_t rstPin) : mfrc522(ssPin, rstPin)
{
  this->ssPin = ssPin;
  this->rstPin = rstPin;

  SPI.begin();
  mfrc522.PCD_Init();
  delay(4);
  mfrc522.PCD_DumpVersionToSerial();
}

std::shared_ptr<railschema::Event> RFIDReader::Loop()
{
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return nullptr;
  }

  if (!mfrc522.PICC_ReadCardSerial())
  {
    return nullptr;
  }

  // mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

  auto event = std::make_shared<railschema::Event>();

  event->type = railschema::EventType::TRAIN;
  std::string value = "";

  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    if (mfrc522.uid.uidByte[i] < 0x10)
      value += " 0";
    else
      value += " ";
    value += std::to_string(mfrc522.uid.uidByte[i]);
  }

  event->vakue = value;

  return event;
}