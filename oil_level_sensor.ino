#include <HCSR04.h>
#include <ESP8266HTTPClient.h>
#include <Arduino_JSON.h>
#include <ArduinoWiFiServer.h>
#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiGratuitous.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiServerSecureBearSSL.h>
#include <WiFiUdp.h>


HCSR04 hc(13, 12); // Initialize Pin D7, D6

String serverName = "http://xx:4567/oil-level";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin("xxxx", "xx");

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  // put your main code here, to run repeatedly:
  int delaytime = 100;
  Serial.println(hc.dist()); // Print in centimeters the value from the sensor
  if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      http.begin(client, serverName);
      http.addHeader("Content-Type", "application/json");
      String jsonString = "{\"level\":\"" + String(hc.dist()) + "\"}";
      if((String(hc.dist()))!= "0.00") {
          int httpResponseCode = http.POST(jsonString);
          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);
          //int delaytime = 86400000;
          int delaytime = 9000;
      }
        
      // Free resources
      http.end();
    }
  delay(delaytime);
}
