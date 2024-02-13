#pragma once

#include <vector>
#include <memory>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#else
#include <WiFi.h>
#include <ESPmDNS.h>
#endif
#include <PubSubClient.h>
#include <arduino-timer.h>
#include "BaseDevice.h"

class ControlUnit
{
  void reconnect();
  WiFiClient espClient;
  PubSubClient client;
  std::string wifiNetwork;
  std::string wifiPassword;
  std::string mqttServer;
  std::string mqttClientId;
  std::string mqttLogin;
  std::string mqttPassword;

  static bool send_report(void *argument);

public:
  ControlUnit(std::string wifiNetwork, std::string wifiPassword, std::string mqttServer, std::string mqttClientId, std::string mqttLogin, std::string mqttPassword);
  void Setup();
  void Loop();

  static void callback(char *topic, byte *payload, unsigned int length);

  std::vector<std::shared_ptr<BaseDevice>> devices;
  std::string id;
};
