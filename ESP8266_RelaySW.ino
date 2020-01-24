#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h> //This line is for OTA support, do not delete or change

//  FW Ver 02, Firmware for Dishwasher with ON pulse using 5v Relay module

MDNSResponder mdns;

const char* ssid = "xxx";
const char* password= "yyy";

ESP8266WebServer server(80);
String webPage = "";
String webPageON = "";
String webPageOFF = "";

//GPIO & LED pins
const int WiFiLed = 2;     
const int Relay = 0;

void setup(){
//These lines are for OTA support, do not delete or change*****************************
  ArduinoOTA.onError([](ota_error_t error) { ESP.restart(); });
  ArduinoOTA.begin();
//*************************************************************************************

//Insert sketch code from here*********************************************************

//Web Page Design
  webPage += "<h1 style='text-align:center;'>Dishwasher 88</h1><h2 style='text-align:center;'>(fw ver 02)</h2><p style='text-align:center;'><a href=\"on\"><button style='font-size:170%;'>ON</button></a>&nbsp;&nbsp<a href=\"off\"><button style='font-size:170%;'>OFF</button></a></p><p style='text-align:center;'><button style='font-size:170%;'>STATUS</button></p>";  
  webPageON += "<h1 style='text-align:center;'>Dishwasher 88</h1><h2 style='text-align:center;'>(fw ver 02)</h2><p style='text-align:center;'><a href=\"on\"><button style='font-size:170%;'>ON</button></a>&nbsp;&nbsp<a href=\"off\"><button style='font-size:170%;'>OFF</button></a></p><p style='text-align:center;'><button style='font-size:170%;background-color:lime;'>STATUS</button></a></p>";  
  webPageOFF += "<h1 style='text-align:center;'>Dishwasher 88</h1><h2 style='text-align:center;'>(fw ver 02)</h2><p style='text-align:center;'><a href=\"on\"><button style='font-size:170%;'>ON</button></a>&nbsp;&nbsp<a href=\"off\"><button style='font-size:170%;'>OFF</button></a></p><p style='text-align:center;'><button style='font-size:170%;background-color:red;'>STATUS</button></a></p>";  

// Initialise
  pinMode(WiFiLed, OUTPUT);
  pinMode(Relay, OUTPUT);
  digitalWrite(WiFiLed, HIGH); //OFF
  digitalWrite(Relay, HIGH); //OFF

  Serial.begin(115200); 
  //  delay(3000); //was 5000
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
  
  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
  server.on("/", [](){
    if(digitalRead(Relay)==LOW) { //If relay is ON
      server.send(200, "text/html", webPageON);
    } else {
      server.send(200, "text/html", webPageOFF);
    }
  });
  server.on("/on", [](){ //pulse ON for 500ms
    server.send(200, "text/html", webPageON);
    digitalWrite(Relay, LOW); //ON
    delay(1000);
    digitalWrite(Relay, HIGH); // OFF
  });
  server.on("/off", [](){
    server.send(200, "text/html", webPageOFF);
    digitalWrite(Relay, HIGH); //OFF
    delay(1000); 
  });
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(){
  ArduinoOTA.handle(); //This line is for OTA support, do not delete or change position

//Insert sketch code from here*********************************************************

  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(WiFiLed, LOW);
  }
  else{
    digitalWrite(WiFiLed, HIGH);
  }
  server.handleClient();
} 
