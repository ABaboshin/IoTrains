using System.Security.AccessControl;
using System.Text.Json.Serialization;
using uPLibrary.Networking.M2Mqtt;
using uPLibrary.Networking.M2Mqtt.Messages;

string broker = "127.0.0.1";
int port = 1883;
string topic = "inTopic";
string clientId = "manager";
// If the broker requires authentication, set the username and password
string username = "manager";
string password = "!Passw0rd";

static MqttClient ConnectMQTT(string broker, int port, string clientId, string username, string password)
{
    MqttClient client = new MqttClient(broker, port, false, MqttSslProtocols.None, null, null);
    client.Connect(clientId, username, password);
    if (client.IsConnected)
    {
        Console.WriteLine("Connected to MQTT Broker");
    }
    else
    {
        Console.WriteLine("Failed to connect");
    }
    return client;
}

static void Publish(MqttClient client, string topic)
{
    int msg_count = 0;
    while (true)
    {
        System.Threading.Thread.Sleep(1*1000);
        client.Publish("d1", System.Text.Encoding.UTF8.GetBytes(System.Text.Json.JsonSerializer.Serialize(new Command{CommandType = CommandType.Move, Value = "fwd"})));
        // Console.WriteLine("Send `{0}` to topic `{1}`", msg, topic);
        msg_count++;
    }
}



try
{

    MqttClient client = ConnectMQTT(broker, port, clientId, username, password);
// Publish(client, topic);

client.MqttMsgPublishReceived += (sender, e) => {
    var msg = System.Text.Encoding.UTF8.GetString(e.Message);
    Console.WriteLine(msg);
    var cu = System.Text.Json.JsonSerializer.Deserialize<ControlUnit>(msg);
    client.Publish(cu.Devices[0].Id, System.Text.Encoding.UTF8.GetBytes(System.Text.Json.JsonSerializer.Serialize(new Command{CommandType = CommandType.Move, Value = "fwd"})));
};
// client.Subscribe(new []{"outTopic"}, new byte[]{ MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE });    

Publish(client, topic);
Console.ReadLine();
}
catch (System.Exception e)
{
    Console.WriteLine(e.Message);
    throw;
}

public enum Function
{
    Default = 0,
    Music = 1
}

public enum DeviceType
{
    Train = 0,
    Turnout = 1
}

public class Device
{
    [JsonPropertyName("id")]
    public string Id { get; set; }
    // public DeviceType Type { get; set; }
    [JsonPropertyName("functions")]
    public List<Function> Functions { get; set; }
}

public class ControlUnit
{
    [JsonPropertyName("id")]
    public string Id { get; set; }
    [JsonPropertyName("devices")]
    public List<Device> Devices { get; set; }
}

public enum CommandType
{
    Move = 0
}

public class Command
{
    [JsonPropertyName("commandtype")]
    public CommandType CommandType { get; set; }
    [JsonPropertyName("value")]
    public string Value { get; set; }
}