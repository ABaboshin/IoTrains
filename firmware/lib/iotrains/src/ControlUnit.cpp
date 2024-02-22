#include <Arduino.h>

#include "ControlUnit.h"

ControlUnit *instance;

const std::uint32_t defaultWaitIntervalMs = 500;
auto timer = timer_create_default();

bool ControlUnit::send_report(void *argument)
{
  nlohmann::json j;

  railschema::ControlUnit cu;
  cu.id = instance->id;
  for (auto it = instance->devices.begin(); it != instance->devices.end(); it++)
  {
    railschema::Device d;
    d.capabilities = it->get()->capabilities;
    d.id = it->get()->id;
    cu.devices.push_back(d);
  }

  to_json(j, cu);
  instance->client.publish("report", j.dump().c_str());
  return true;
}

ControlUnit::ControlUnit(std::string wifiNetwork, std::string wifiPassword, std::string mqttServer, std::string mqttClientId, std::string mqttLogin, std::string mqttPassword, std::uint32_t connectionTimeoutMs) : client(espClient),
                                                                                                                                                                                                                    wifiNetwork(wifiNetwork),
                                                                                                                                                                                                                    wifiPassword(wifiPassword),
                                                                                                                                                                                                                    mqttServer(mqttServer),
                                                                                                                                                                                                                    mqttClientId(mqttClientId),
                                                                                                                                                                                                                    mqttLogin(mqttLogin),
                                                                                                                                                                                                                    mqttPassword(mqttPassword),
                                                                                                                                                                                                                    connectionTimeoutMs(connectionTimeoutMs)
{
  instance = this;
}

void ControlUnit::Setup()
{
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifiNetwork.c_str());

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiNetwork.c_str(), wifiPassword.c_str());

  std::uint32_t wait = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    wait += defaultWaitIntervalMs;
    delay(defaultWaitIntervalMs);
    Serial.print(".");
    if (wait > connectionTimeoutMs) {
      Serial.println("wifi not connected");
      devices[0]->DefaultAction();
      return;
    }
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp"))
  {
    Serial.println("MDNS responder started");
  }
  else
  {
    Serial.println("MDNS responder not started");
  }

  IPAddress ip;
  ip.fromString(mqttServer.c_str());
  client.setServer(ip, 1883);
  client.setCallback(callback);

  timer.every(5000, ControlUnit::send_report);
}

void ControlUnit::callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  std::string s((const char *)payload, length);
  Serial.println(s.c_str());
  const auto j = nlohmann::json::parse(s);

  auto cmd = railschema::from_json<railschema::Command>(j);

  for (auto i = 0; i < instance->devices.size(); i++) {
    if (instance->devices[i]->id == topic)
    {
      Serial.println("process");
      auto state = (*instance->devices[i]).ProcessCommand(cmd);
      if (state != nullptr) {
        delay(500);
        nlohmann::json j;
        railschema::to_json<railschema::State>(j, state);
        Serial.println(j.dump().c_str());
        instance->client.publish("state", j.dump().c_str());
      }

      break;
    }
  }
}

void ControlUnit::Loop()
{
  if (!client.connected())
  {
    reconnect();
  }

  client.loop();
  timer.tick();

  for (std::size_t i = 0; i < devices.size(); i++)
  {
    auto event = devices[i]->Loop();
    if (event != nullptr)
    {
      nlohmann::json j;
      railschema::to_json<railschema::Event>(j, event);
      client.publish("event", j.dump().c_str());
    }
  }

  delay(10);
}

void ControlUnit::reconnect()
{
  std::uint32_t wait = 0;
  while (!client.connected())
  {
    Serial.printf("Attempting MQTT connection... %s %s %s", mqttClientId, mqttLogin, mqttPassword);
    if (client.connect(mqttClientId.c_str(), mqttLogin.c_str(), mqttPassword.c_str()))
    {
      Serial.println("connected");
      for (auto i = this->devices.begin(); i != this->devices.end(); i++)
      {
        client.subscribe(i->get()->id.c_str());
      }
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again later");
      delay(defaultWaitIntervalMs);
      wait += defaultWaitIntervalMs;
      if (wait > connectionTimeoutMs) {
        Serial.println("wifi not connected");
        devices[0]->DefaultAction();
        return;
      }
    }
  }
}
