#include <Arduino.h>
#include "WiFi.h"

// Complete Instructions to Get and Change ESP MAC Address: 
//https://RandomNerdTutorials.com/get-change-esp32-esp8266-mac-address-arduino/
void setup(){
  Serial.begin(115200);
  WiFi.mode(WIFI_MODE_STA);
  Serial.println(WiFi.macAddress());
}
  
void loop(){

}