#include <Arduino.h>
#include "humsol.hpp"
#include "pluie.hpp"  //capteur de pluie
#include "ldr.hpp"
#include <esp_now.h>
#include <WiFi.h>
#include "DHT.h"

int redLED=25;          //sol led
int solPin=35;            //sol Pin
#define sensorPin 32            //pluie Pin
int ldrPin = 34;      //ldr Pin
#define DHT11PIN 33
DHT dht(33, DHT11);

uint8_t broadcastAddress[] = {0x7C, 0x9E, 0xBD, 0xE5, 0x22, 0x94};
  // Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  float solval;                 //valeur d'humidité de sol
  float pluie;
  float ldr;
   float humi;
  float temp;
} struct_message;

struct_message myData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nSend message status:\t");

  if(status == ESP_NOW_SEND_SUCCESS){
  Serial.println("Send Successfully");
  }else{
    Serial.println("Send Failed");
  }
}  

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);

/* Start the soil moister Sensor */
   pinMode(solPin, INPUT);
  pinMode(redLED, OUTPUT);
  /* Start rain Sensor */
    pinMode(sensorPin, INPUT);
/* Start ldr Sensor */
  pinMode(ldrPin, INPUT);
/* Start the DHT11 Sensor */

  dht.begin();

// Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("There was an error initializing ESP-NOW");
    return;
  }
 // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;  
    }

}
void loop(){

  // put your main code here, to run repeatedly:
  myData.solval= setuphumsol(solPin,redLED);
  myData.pluie=readpluieSensor(sensorPin);
  myData.ldr=readldrSensor(ldrPin);
  myData.humi = dht.readHumidity();
  myData.temp = dht.readTemperature();
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println(myData.solval);
    Serial.println("Send with success");
  }
  else {
    Serial.println("Error sending the data");
  }
}
