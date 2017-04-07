// Written by Pete Sagat
// Date: 3/31/17
// Program: Sketch to gather detect if you have a water leak. If water is detected an alert is sent
// to pushbullet. A health check is run every one day to verify the device is still listening.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define LEAK_PIN  5  // Arduino Digital I/O pin number
int lastLeak = 1;

unsigned long previousMillisLeak = 0;
unsigned long previousMillisHealth = 0;

int leakCheckInterval = 15000; // check for leaks every 15 seconds
int healthCheckInterval = 86400000; // runs a health check every 1 minute so we know we haven't died

const char* ssid     = "SSID";
const char* password = "*******";

const char* host = "api.pushbullet.com";
//https://api.pushbullet.com/v2/pushes
const int httpsPort = 443;

const char* accessToken = "SECRET"; // PushBullet API for account
const char* fingerprint = "‎‎45 cf bc d7 cc ca 78 b3 43 ed 1d e9 c2 97 96 e6 f9 81 5a 4e"; // PushBullet SSL Site cert

// Not used but can be for mqtt
// const char* mqtt_server = "192.168.1.126"; 
// const char* mqtt_topic = "sensors/test/water";

void healthCheck (){
// get current time stamp
unsigned long currentMillis = millis();
  if (currentMillis - previousMillisHealth >= healthCheckInterval) {
       previousMillisHealth = currentMillis;

   Serial.println("It's been 60 minutes, sending health check");
   // Use WiFiClientSecure class to create TLS connection
   WiFiClientSecure client;
   Serial.print("connecting to ");
   Serial.println(host);
   if (!client.connect(host, httpsPort)) {
   Serial.println("connection failed");
   return;
   }
   // SSL cert, pull from cert on site
   if (client.verify(fingerprint, host)) {
   Serial.println("certificate matches");
   } 
   else {
   Serial.println("certificate doesn't match");
   }
   
  String url = "/v2/pushes";
  Serial.print("requesting URL: "); Serial.println(url);
  String body= "{\"body\":\"Health Check (Basement Furnace)\",\"title\":\"Online\",\"type\":\"note\"}";
  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: ESP8266\r\n" +
               "Access-Token: " + accessToken + "\r\n" +
               "Content-length: " + body.length() +"\r\n" 
               "Content-Type: application/json\r\n" +
               "Connection: close\r\n\r\n" +
               body
              );


  Serial.println("request sent");
    while (client.connected()) {
    String line = client.readStringUntil('\n');
      if (line == "\r") {
       Serial.println("headers received");
       break;
      }
    }
    String line = client.readStringUntil('\n');
    //String line = client.readString(); // this line is good for debugging error messages more then a single line
    if (line.startsWith("{\"active\":true")) {
      Serial.println("esp8266/Arduino CI successfull!");
    } else {
      Serial.println("esp8266/Arduino CI has failed");
    }
    Serial.println("reply was:");
    Serial.println("==========");
    Serial.println(line);
    Serial.println("==========");
    Serial.println("closing connection");
    Serial.println("Pausing for 30 seconds..."); 
   }
  }

void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(LEAK_PIN, INPUT);  
    
// We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
// Give us the status of the Wifi connection

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
// Set current time
unsigned long currentMillis = millis();
if (currentMillis - previousMillisLeak >= leakCheckInterval) { // Only runs if 15 seconds have passed since the last run
    previousMillisLeak = currentMillis;
    // Read digital pin value (D1 on esp8266 nodemcu) Any digital input would work though.
    int leakValue = digitalRead(LEAK_PIN);
    Serial.println("Current Leak Value");
    Serial.println(leakValue);
    Serial.println(lastLeak);
    Serial.println();
    
    // Compares leak sensor to what was read last iteration
    if (lastLeak != leakValue){
    
        if (leakValue == 0) {
          Serial.println("Water Detected!!!!!");
          Serial.println(leakValue);
          // Let's send this bad news to push bullet!!!
        
          // Use WiFiClientSecure class to create TLS connection
          WiFiClientSecure client;
          Serial.print("connecting to ");
          Serial.println(host);
          if (!client.connect(host, httpsPort)) {
            Serial.println("connection failed");
            return;
          }
         // SSL cert, pull from cert on site
          if (client.verify(fingerprint, host)) {
            Serial.println("certificate matches");
          } else {
            
            Serial.println("certificate doesn't match");
            
          }
    
          String url = "/v2/pushes";
          Serial.print("requesting URL: "); Serial.println(url);
          String body= "{\"body\":\"Water has been detected by location (Basement Furnace)\",\"title\":\"Water Detected\",\"type\":\"note\"}";
          client.print(String("POST ") + url + " HTTP/1.1\r\n" +
                       "Host: " + host + "\r\n" +
                       "User-Agent: ESP8266\r\n" +
                       "Access-Token: " + accessToken + "\r\n" +
                       "Content-length: " + body.length() +"\r\n" 
                       "Content-Type: application/json\r\n" +
                       "Connection: close\r\n\r\n" +
                       body
                      );
        
        
          Serial.println("request sent");
          while (client.connected()) {
            String line = client.readStringUntil('\n');
            if (line == "\r") {
              Serial.println("headers received");
              break;
            }
          }
          String line = client.readStringUntil('\n');
          //String line = client.readString(); // this line is good for debugging error messages more then a single line
          if (line.startsWith("{\"active\":true")) {
            Serial.println("esp8266/Arduino CI successfull!");
          } else {
            Serial.println("esp8266/Arduino CI has failed");
          }
          Serial.println("reply was:");
          Serial.println("==========");
          Serial.println(line);
          Serial.println("==========");
          Serial.println("closing connection");
          Serial.println("Pausing for 30 seconds...");
        }
        else {
          Serial.println("No water detected, all is well.");
        }
        
      }

    // Don't send another alert if one has already been generated
    else if (leakValue == 0) {
          Serial.println("Water Detected!!!!!");
          Serial.println("Alert already sent, will not send another.");
      }
  
    else {
    Serial.println("No water detected, all is well.");
   }
 // Sets lastLeak to last value detected by sensor
 lastLeak = leakValue;

}
  // Runs healthcheck function
  healthCheck();
}


