#include <Arduino.h>
#include "blynk.hpp"

#define BLYNK_TEMPLATE_ID "TMPLv74UaZh9"
#define BLYNK_DEVICE_NAME "Myserre"
#define BLYNK_PRINT Serial   
#include <SPI.h>
 #include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <WiFiClient.h>
#include <Wire.h>


char auth[] ="_gSbYMByVp52DbGJZSC85n-1ne24hq1M";               //Authentication code sent by Blynk
char ssid[] = "WLAN-ETUD";                       //WiFi SSID  WLAN-ETUD
char pass[] = "isetetud2122";                      //isetetud2122

  

void blynk(float temp,float humi){
    Blynk.begin(auth,ssid,pass);
    Blynk.run();
    Blynk.virtualWrite(V0,temp);
    Blynk.virtualWrite(V1,humi);

}

void blynk(float sol){
    Blynk.begin(auth,ssid,pass);
    Blynk.run();
   Blynk.virtualWrite(V4,sol);
if(sol > 30){
   Serial.println("-- votre plante n'a pas besoin d'eau--");
  Blynk.logEvent("WATER","-- votre plante n'a pas besoin d'eau--");
  }
  else {
    Serial.println("-- votre plante a besoin d'eau --");
    Blynk.logEvent("WATER","-- votre plante besoin d'eau --");
    }
}