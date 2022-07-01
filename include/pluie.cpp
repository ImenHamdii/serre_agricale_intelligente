#include <Arduino.h>
#include "pluie.hpp"

float readpluieSensor(int sensorPin){ //
  int pluieValue = analogRead(sensorPin);  // Read the analog value from sensor
    float outputValue = map(pluieValue, 0, 1023, 255, 0); // map the 10-bit data to 8-bit data
   Serial.println(outputValue);  
  
if(isnan(outputValue)) 
  {
    Serial.println(F("Failed to read from pluie Sensor!"));
    return 0;
  }
else
  {
    return outputValue;
  }
}