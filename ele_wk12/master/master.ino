#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Wire.h>

const char* ssid = "IOTtest";
const char* password = "";

ESP8266WebServer server(80);
const int led = BUILTIN_LED;
char led_status[25];
String webSite;
byte x = 0;



void buildWeb() {
  webSite+="<html>\n";
  webSite+="<head>\n";
  webSite+="<meta charset='UTF-8'/>\n";
  webSite+="<title>Form Control LED</title>\n";
  webSite+="<style>\n";
  webSite+="body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\n";
  webSite+="div{width: 500px;  text-align: center;margin:50 auto;}\n";
  webSite+="</style>\n";
  webSite+="</head>\n";
  webSite+="<body>\n";
  webSite+="<div>\n";
  webSite+="<h1>LED Control Form ~</h1>\n";
  webSite+="<p id=\"ledstatus\">You can select a LED status ...</p>\n";
  webSite+="<form action=\"/form1\">\n";
  webSite+="<input type=\"radio\" id=\"on\" name=\"led\" value=\"on\"><label for=\"on\"> LED ON  </label>\n";
  webSite+="<input type=\"radio\" id=\"off\" name=\"led\" value=\"off\"><label for=\"off\"> LED OFF </label><br><br>\n";
  webSite+="<input type=\"submit\" value=\"submit\"></form>\n";
  webSite+="</div>\n";
  webSite+="</body>\n";
  webSite+="</html>\n";      
}


void handleRoot() {
   server.send ( 200, "text/html", webSite ); //send back client a HTML file

}
void handleLEDStatus() {
  if (server.arg("led") == "off") {
  snprintf(led_status, 25, "Now, LED is OFF ...");
  digitalWrite(led, 1);
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write("x is ");        // sends five bytes
  Wire.write(1);              // sends one byte
  Wire.endTransmission();    // stop transmitting
  }
  else {
  snprintf(led_status, 25, "Now, LED is ON ...");
  digitalWrite(led,0);
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write("x is ");        // sends five bytes
  Wire.write(0);              // sends one byte
  Wire.endTransmission();    // stop transmitting
  }
  server.send ( 200 , "text/html" , led_status );
}



void handleNotFound(){
  server.send ( 404, "text/html", "404 Error!" );  

}

void setup(void){
  buildWeb();
  pinMode ( led, OUTPUT );
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  server.on ( "/", handleRoot );
  server.on("/form1",handleLEDStatus);
  server.onNotFound ( handleNotFound );
  Wire.begin(4,5);
  server.begin();

}

void loop(void){
  server.handleClient();
}
