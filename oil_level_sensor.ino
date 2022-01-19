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
int clockUpdateCounter = 1440;
String serverName = "http://192.168.1.xxx/oil-level";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin("xxx", "xxx");

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
  int delaytime = 10000;
  if (clockUpdateCounter == 1440) {
  String distance_reading = "0.00";
    while (distance_reading == "0.00") {
      // put your main code here, to run repeatedly:
      distance_reading = String(hc.dist());
      //Serial.println(hc.dist()); // Print in centimeters the value from the sensor
      if(WiFi.status()== WL_CONNECTED){
        if(distance_reading!= "0.00") {
          WiFiClient client;
          HTTPClient http;
          http.begin(client, serverName);
          http.addHeader("Content-Type", "application/json");
          String jsonString = "{\"level\":\"" + distance_reading + "\"}";
          int httpResponseCode = http.POST(jsonString);
          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);
          Serial.print(distance_reading);
          // Free resources
          http.end();
          //delaytime = 10000;
          if (httpResponseCode == 200) {
            clockUpdateCounter = 0;
          }
          else {
            clockUpdateCounter = 1439;
          }
        }
        else {
          clockUpdateCounter = 1439;
        }
      }
    }
  }
  clockUpdateCounter++;
  delay(delaytime);
}
