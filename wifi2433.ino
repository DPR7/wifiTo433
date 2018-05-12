#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <RCSwitch.h>

RCSwitch bell = RCSwitch();

const char* ssid = "YOUR WIFI SSID";
const char* password = "YOURWIFIPASSWORD";

ESP8266WebServer server(80);

void setup(void){
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

   // Transmitter is connected to Arduino Pin #2 
   bell.enableTransmit(2);

   // Optional set pulse length.
   bell.setPulseLength(334);

   // Optional set number of transmission repetitions.
   bell.setRepeatTransmit(24);

  server.on("/siren", [](){
    //If a post is received, ring the siren
    if (server.method() == HTTP_POST){
      Serial.println("Sending siren signal");
      bell.send("000010010111110110101000");
      server.send(200, "text/plain", "{'result': 'ok'}");
    }
    //Home assistant RESTful Switch will regularly check the status with a GET request.
    //Always return false, because the siren will only be "on" for a few microseconds
    if (server.method() == HTTP_GET){
      Serial.println("Returning status message");
      server.send(200, "text/plain", "{'active': 'false'}");
    }
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}