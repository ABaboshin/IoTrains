#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <google/protobuf/util/time_util.h>

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

void setup_wifi() {

  delay(10);
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
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
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
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
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