//
// Created by andrey.baboshin on 1/31/2024.
//

#include <Arduino.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#else
#include <WiFi.h>
#include <ESPmDNS.h>
#endif
#include <PubSubClient.h>

#include "ControlUnit.h"
#include "config.h"

ControlUnit* instance;

WiFiClient espClient;
PubSubClient client(espClient);

ControlUnit::ControlUnit () {
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

  if (MDNS.begin("esp8266"))
  {
    Serial.println("MDNS responder started");
  } else {
    Serial.println("MDNS responder not started");
  }

  IPAddress ip;
  ip.fromString(mqtt_server);
  client.setServer(ip, 1883);
  client.setCallback(callback);
}

void ControlUnit::callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");

    std::string s ((const char*)payload, length);
    Serial.println(s.c_str());
    const auto j = nlohmann::json::parse(s);
    railschema::Command cmd;
    from_json(j, cmd);

    for (auto i = instance->get_devices().begin(); i != instance->get_devices().end(); i++)
    {
      if (i->get_id() == topic)
      {
        (*i).ProcessCommand(cmd);
        break;
      }
    }
}

void ControlUnit::Loop() {
  if (!client.connected()) {
      reconnect();
  }

  client.loop();
}

void ControlUnit::reconnect() {
    while (!client.connected()) {
      Serial.printf("Attempting MQTT connection... %s %s %s", mqtt_clientid, mqtt_login, mqtt_password);
      if (client.connect(mqtt_clientid, mqtt_login, mqtt_password))
      {
        Serial.println("connected");
        for (auto i = this->get_devices().begin(); i != this->get_devices().end(); i++)
        {
          client.subscribe(i->get_id().c_str());
        }

        nlohmann::json j;
        to_json(j, *this);
        client.publish("report", j.dump().c_str());
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}
