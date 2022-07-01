#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <Servo.h>
#include "blynk.hpp"  //code blynk 

int pompe=25;          //pompe pin
int moteurPin=26;      //pluie moteur
int ledldr = 13;      //ldr led
    Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards
  int pos = 0;    // variable to store the servo position
int ventilateurPin=33;      //ventilateur Pin

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  float solval;
  float pluie;
  float ldrValue;
 float humi;
  float temp;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
  Serial.println(macStr);
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("solval %: ");
  Serial.println(myData.solval);
  Serial.print("pluie: ");
  Serial.println(myData.pluie);
  Serial.print("ldrValue: ");
  Serial.println(myData.ldrValue);
  Serial.print("humi: ");
  Serial.println(myData.humi);
  Serial.print("temp: ");
  Serial.println(myData.temp);
  Serial.println();
  delay(200);
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  
  pinMode(pompe, OUTPUT);
    pinMode(ledldr, OUTPUT);
    pinMode(ventilateurPin, OUTPUT);
    digitalWrite(pompe, LOW);
myservo.attach(moteurPin);  // attaches the servo on pin 13 to the servo object
   
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  //code capteur d'humidité de sol
  
    float sol=myData.solval;
if(myData.solval > 30){
    Serial.println(sol);
    digitalWrite(pompe, LOW);
 blynk(sol);

  }
  else {  
    digitalWrite(pompe, HIGH);
    blynk(sol);
    Serial.println(sol);
    }
  //code ldr
  float ldrValue=myData.ldrValue;
  if(ldrValue >= 4000) 
  {
    digitalWrite(ledldr, HIGH);
Serial.print("Its DARK, Turn on LED  ");
Serial.println(ldrValue);
  }
else
  {
   digitalWrite(ledldr, LOW);
Serial.print("Its BRIGHT, Turn off LED  ");
Serial.println(ldrValue);
  }

  //code capteur pluie
  float pluie=myData.pluie;
  if(myData.pluie>-200) 
  {
    Serial.println("-- fermer serre --"); 
    Serial.println(myData.pluie);
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
  }
  }
else
  {
    Serial.println("-- ouvrir serre --");
    Serial.println(myData.pluie);
     for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
  }
  }

//dht code
float humi=myData.humi;
float temp=myData.temp;
  if ( (humi>80) || (temp>37) ) 
{ 
  Serial.println("ouvrir ventilateur ");
blynk(temp,humi);
digitalWrite(ventilateurPin, HIGH); 

}
else 
{
  Serial.println("ventilateur fermer ");
digitalWrite(ventilateurPin, LOW); 
blynk(temp,humi);
}
}