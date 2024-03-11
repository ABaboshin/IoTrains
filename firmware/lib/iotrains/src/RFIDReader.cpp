#include "RFIDReader.h"
#include <Arduino.h>
#include <memory>

static bool spiInitialized = false;

RFIDReader::RFIDReader(const std::string &id, std::uint8_t ssPin, std::uint8_t rstPin) : BaseDevice(id) //, mfrc522(ssPin, rstPin)
{
  this->ssPin = ssPin;
  this->rstPin = rstPin;
  if (!spiInitialized)
  {
    Serial.println("spi init");
    SPI.begin();
    spiInitialized = true;
    pinMode(rstPin, OUTPUT);
    digitalWrite(rstPin, HIGH);
  }

  pinMode(ssPin, OUTPUT);
  digitalWrite(ssPin, HIGH);

  railschema::Capability capability;
  capability.type = railschema::CapabilityType::DETECTOR;
  capability.value = "";
  capabilities.push_back(capability);
}


void RFIDReader::Init()
{
  Serial.print(id.c_str());
  Serial.println(" init");
  mfrc522.PCD_Init(ssPin, rstPin);
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

  std::shared_ptr<railschema::Event> event = std::make_shared<railschema::Event>();

  event->type = railschema::EventType::TRAIN;
  event->source = id;
  std::string value = "";

  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    if (mfrc522.uid.uidByte[i] < 0x10)
      value += " 0";
    else
      value += " ";
    value += std::to_string(mfrc522.uid.uidByte[i]);
  }

  value.erase(value.begin(), std::find_if(value.begin(), value.end(), [](unsigned char ch) { return !std::isspace(ch); }));

  Serial.println(value.c_str());

  event->value = value;

  return event;
}
