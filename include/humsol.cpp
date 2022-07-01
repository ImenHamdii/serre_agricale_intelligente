#include <Arduino.h>
#include "humsol.hpp"


float setuphumsol(int solPin,int redLED){

  int solsensorValue = analogRead(solPin);
float solval= (100-((solsensorValue/4096.00)*100));

   if (isnan(solsensorValue)) {
    Serial.println(F("Failed to read from Soil Humidity sensor!"));
    return 0;
  }else{
    return solval;
  }
 
}