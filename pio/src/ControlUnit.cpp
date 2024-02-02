#include <Arduino.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#else
#include <WiFi.h>
#include <ESPmDNS.h>
#endif
#include <PubSubClient.h>
#include <arduino-timer.h>

#include "ControlUnit.h"
#include "config.h"

ControlUnit *instance;

WiFiClient espClient;
PubSubClient client(espClient);
auto timer = timer_create_default();

bool send_report(void *argument)
{
  // nlohmann::json j;
  // to_json(j, *instance);
  // client.publish("report", j.dump().c_str());
  return true;
}

ControlUnit::ControlUnit()
{
  instance = this;
}

void ControlUnit::Setup()
{
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
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
  ip.fromString(mqtt_server);
  client.setServer(ip, 1883);
  client.setCallback(callback);

  timer.every(5000, send_report);
}

void ControlUnit::callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  std::string s((const char *)payload, length);
  Serial.println(s.c_str());
  const auto j = nlohmann::json::parse(s);
  railschema::Command cmd;
  from_json(j, cmd);

  for (auto i = 0; i < instance->devices.size(); i++) {
    if (instance->devices[i]->id == topic)
    {
      Serial.println("process");
      auto state = (*instance->devices[i]).ProcessCommand(cmd);
      nlohmann::json j;
      state->to_json(j);
      client.publish("state", j.dump().c_str());
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
  delay(100);
}

void ControlUnit::reconnect()
{
  while (!client.connected())
  {
    Serial.printf("Attempting MQTT connection... %s %s %s", mqtt_clientid, mqtt_login, mqtt_password);
    if (client.connect(mqtt_clientid, mqtt_login, mqtt_password))
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
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
