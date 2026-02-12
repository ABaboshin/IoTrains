#include <Arduino.h>
#include <memory>
#include <ControlUnit.h>
#include "MP3Player.h"
#include "SoundTrain.h"

#include "config.h"

#include "helpers.h"
#include "mp3.h"

ControlUnit cu(wifiNetwork, wifiPassword, mqttServer, mqttClientId, mqttLogin, mqttPassword, 1000 * 5);

// Motor A
int motor1Pin1 = 1;
int motor1Pin2 = 2;
int enable1Pin = 3;

// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 200;

void setup()
{
  Serial.begin(115200);
  delay(10);
  cu.id = STRINGIZE_VALUE_OF(NAME);

  Serial.println("train");

  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  ledcAttachPin(enable1Pin, pwmChannel);
  // ledcAttachChannel(enable1Pin, freq, resolution, pwmChannel);


  // std::map<std::string, MP3File> sounds;
  // sounds["test"].data = outputmp3;
  // sounds["test"].size = outputmp3_size;
  // = {outputmp3, outputmp3_size}; //(outputmp3, outputmp3 + sizeof(outputmp3));
  // std::shared_ptr<BaseDevice> train = std::make_shared<SoundTrain>(STRINGIZE_VALUE_OF(NAME), 1, 2, 3, 4, 0, sounds);

  // train->DefaultAction();
  // std::shared_ptr<BaseDevice> train = std::make_shared<Train>(STRINGIZE_VALUE_OF(NAME), 1, 2, 3, 4);

  // train->DefaultAction();
  // cu.devices.push_back(train);

  // cu.Setup();

  // digitalWrite(1, 0);
  // digitalWrite(2, 1);

  // int _maxpwm = 255;

  // pinMode(1, OUTPUT);
  // pinMode(2, OUTPUT);

  // analogWrite(1, 0);
  // analogWrite(2, 255);
}

void loop()
{
  // Move the DC motor forward at maximum speed
  Serial.println("Moving Forward");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  delay(3000);

  // Stop the DC motor
  Serial.println("Motor stopped");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  delay(3000);

  // Move DC motor backwards at maximum speed
  Serial.println("Moving Backwards");
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  delay(3000);

  // Stop the DC motor
  Serial.println("Motor stopped");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  delay(3000);

  // Move DC motor forward with increasing speed
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  while (dutyCycle <= 255){
    ledcWrite(enable1Pin, dutyCycle);
    Serial.print("Forward with duty cycle: ");
    Serial.println(dutyCycle);
    dutyCycle = dutyCycle + 5;
    delay(500);
  }
  dutyCycle = 200;
  // cu.Loop();
}
