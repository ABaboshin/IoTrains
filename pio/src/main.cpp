#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <PubSubClient.h>
// #include "controlunit.h"
// #include <WriteBufferFixedSize.h>
#include "json.hpp"
#include <string>
#include <vector>
#include "schema.hpp"
// #include <google/protobuf/util/time_util.h>

// #include "controlunit.pb.h"
// #include "pb_encode.h"

const char* ssid = "";
const char* password = "";
const char* mqtt_server = "192.168.86.94";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

enum class DeviceType : uint32_t
{
  Train = 0,
  Turnout = 1
};

enum class Function : uint32_t
{
  Default = 0,
  Music = 1
};

enum class CommandType : uint32_t
{
  Move = 0
};

class Device
{
    public:
    std::string id;
    std::vector<Function> functions;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Device, id, functions)
};

class ControlUnit
{
    public:
    std::string id;
    std::vector<Device> devices;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ControlUnit, id, devices)
};

class Command
{
public:
    CommandType commandtype;
    std::string value;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Command, commandtype, value)
};

void setup_wifi() {
  for (int i = 0; i < 20; i++)
  {
    Serial.println("sleep");
    delay(500);
  }

  // Serial.print("MOSI: ");
  // Serial.println(MOSI);
  // Serial.print("MISO: ");
  // Serial.println(MISO);
  // Serial.print("SCK: ");
  // Serial.println(SCK);
  // Serial.print("SS: ");
  // Serial.println(SS);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  std::string s ((const char*)payload, length);
  Serial.println(s.c_str());
  nlohmann::json j = nlohmann::json::parse(s);
  Command cmd;
  from_json(j, cmd);
  Serial.print("Command ");
  Serial.print((int)cmd.commandtype);
  Serial.print(" value ");
  Serial.print(cmd.value.c_str());
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "arduino";
    // clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), "arduino", "!Passw0rd")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
    //   client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("d1");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  // pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  delay(5000);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;

    ControlUnit cu;
    cu.id = "cu1";
    Device d;
    d.id = "d1";
    d.functions.push_back(Function::Default);
    cu.devices.push_back(d);

    nlohmann::json j;
    to_json(j, cu);
    Serial.println(j.dump().c_str());
    client.publish("outTopic", j.dump().c_str());
    // Serial.println(1);
    // delay(500);
    // railway::ControlUnit<100, 100, 100, 100> cu;

    // Serial.println(3);
    // delay(500);
    // // const char * const cuid = "cu1";
    // Serial.println(4);
    // delay(500);
    // EmbeddedProto::FieldString<100> cuids;
    // Serial.println(5);
    // delay(500);
    // cuids = "cu1";
    // Serial.println(6);
    // delay(500);
    // cu.set_id(cuids);
    // Serial.println(7);
    // delay(500);
    // railway::Device<100, 100> device;
    // Serial.println(8);
    // delay(500);
    // const char * const did = "d1";
    // Serial.println(9);
    // delay(500);
    // EmbeddedProto::FieldString<100> dids;
    // Serial.println(10);
    // delay(500);
    // dids = did;
    // Serial.println(11);
    // delay(500);
    // device.set_id(dids);
    // Serial.println(12);
    // delay(500);

    // device.set_type(railway::DeviceType::Train);
    // Serial.println(13);
    // delay(500);
    // device.set_Functions(0, railway::Function::Default);
    // Serial.println(14);
    // delay(500);
    // cu.set_device(0, device);

    // Serial.println(15);
    // delay(500);
    // EmbeddedProto::WriteBufferFixedSize<1000> write_buffer_;
    // Serial.println(16);
    // delay(500);
    // const auto sr = cu.serialize(write_buffer_);

    // client.publish("outTopic", write_buffer_.get_data(), write_buffer_.get_size());
    // railway_ControlUnit cu = railway_ControlUnit_init_default;

    // cu.id = "cu1";
    // cu.device_count = 1;
    // cu.device[0].id = "train1";

    // uint8_t buffer[1000];
    // pb_ostream_t stream;

    // stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

    // if (pb_encode(&stream, railway_ControlUnit_fields, &cu))
    // {
    //   Serial.print("Publish message: ");
    //   client.publish("outTopic", (char*)buffer);
    // } else {
    //   Serial.println("failed to pb_encode");
    // }

    // {
    //   "cu1",
    //   {
    //     {
    //       "train1",
    //       railway_DeviceType_Train,
    //       {
    //         railway_Function_Default
    //       }
    //     }
    //   }
    // };

  }
}
