#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include "ESP8266HTTPClient.h"

#include <WiFiClientSecureBearSSL.h>

ESP8266WiFiMulti WiFiMulti;

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("", "");
}

void loop() {
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

    client->setInsecure();   // client->setFingerprint(fingerprint);
    
    HTTPClient https;

    String url = Serial.readStringUntil('\n');

    if (url.length()>1)
    {
      if (https.begin(*client, url)) {

        int httpCode = https.GET();

        if ((httpCode > 0) &&
            (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)) {
          Serial.println(https.getString());
        }
        else {
          Serial.println();
        }

        https.end();
      } else {
        Serial.println();
      }
    }
  }
}

// // #include "WiFiEsp.h"
// #include <Arduino.h>
// #define WS Serial1


// bool waitFor(const char* str, const unsigned int timeout/*, const FSH * waitfor, bool echo, bool escapeEcho*/)
// {
//   Serial.print(str);
//   WS.print(str);

//   unsigned long  startTime = millis();
//   bool found = false;
  
//   while ( millis() - startTime < timeout) {
//     int nextchar;
//     while (WS.available() && (nextchar = WS.read()) > -1) {
//       char ch = (char)nextchar;
//       Serial.print(ch);
//       found = true;
//     }

//     if (found) {
//       Serial.println();
//       return true;
//     }
//   }

//   Serial.println("nothing to read");
//   return false;
// }

// void setup() {
//   WS.setTX(0);
//   WS.setRX(1);

//   Serial.begin(115200);
//   WS.begin(115200);

//   delay(5000);
//   Serial.println("start...");

//   waitFor("AT\r\n", 200);
//   waitFor("ATE1\r\n", 200);
  
//   waitFor("AT+GMR\r\n", 2000);
//   waitFor("AT+CWMODE_CUR\r\n", 1000);

//   waitFor("AT+CWHOSTNAME=\"rp2040\"\r\n", 2000);
//   waitFor("AT+CWJAP_CUR=\"\",\"\"\r\n", 16000);

//   waitFor("AT+CIFSR\r\n", 5000);
  
// }

// void loop()
// {
//   waitFor("AT\r\n", 200);
//   delay(1000);
// }

// #include <SoftwareSerial.h>

// #define TIMEOUT 5000 // mS

// #define rxPin 2
// #define txPin 3
// SoftwareSerial mySerial(rxPin, txPin);

// boolean SendCommand(String cmd, String ack){
//   Serial.print(cmd);
//   mySerial.print(cmd); // Send "AT+" command to module
//   if (!echoFind(ack)) // timed out waiting for ack string
//     return true; // ack blank or ack found
//   return true;
// }

// boolean echoFind(String keyword){
//  byte current_char = 0;
//  byte keyword_length = keyword.length();
//  long deadline = millis() + TIMEOUT;
//  while(millis() < deadline){
//   if (mySerial.available()){
//     char ch = mySerial.read();
//     Serial.write(ch);
//     if (ch == keyword[current_char])
//       if (++current_char == keyword_length){
//        Serial.println();
//        return true;
//     }
//    }
//   }
//  return false; // Timed out
// }

// void setup()
// {
//   // pinMode(rxPin, INPUT);
//   //   pinMode(txPin, OUTPUT);
//   Serial.begin(115200);
//  mySerial.begin(9600);
//  delay(1000);
//  SendCommand("AT\r\n", "OK");
//  delay(500);
//  SendCommand("ATE1\r\n", "Ready");
//  delay(500);
//   SendCommand("AT+RST\r\n", "Ready");
//  delay(5000);
//  SendCommand("AT+CWHOSTNAME=\"dcc\"\r\n","OK");
//  delay(2000);
//  SendCommand("AT+CWJAP_CUR=\"a\",\"b\"\r\n","OK");
//  SendCommand("AT+CIFSR\r\n", "Ready");
// }

// void loop()
// {}

// #ifdef wav

// #include <PWMAudio.h>
// #include "wav.h"

// // The sample pointers
// const int16_t *start = (const int16_t *)out_raw;
// const int16_t *p = start;

// // Create the PWM audio device on GPIO 1.   Hook amp/speaker between GPIO1 and convenient GND.
// PWMAudio pwm(10);

// unsigned int count = 0;

// void cb() {
//   while (pwm.availableForWrite())
//   {
//     Serial.print(*p);
//     Serial.print(" ");
//     // pwm.write(*p++);
    
//     count += 2;
//     if (count >= sizeof(out_raw)) {
//       count = 0;
//       p = start;
//     }
//   }
// }

// void setup() {
//   Serial.begin(115200);
//   pwm.onTransmit(cb);
//   pwm.begin(44100);
// }

// void loop() {
//   /* noop, everything is done in the CB */
// }
// #endif