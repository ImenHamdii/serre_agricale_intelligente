#include <Arduino.h>
#include "ldr.hpp"

float readldrSensor(int ldrPin){
  int ldrValue = analogRead(ldrPin);  // Read the analog value from sensor
 if (isnan(ldrValue)) {
    Serial.println(F("Failed to read from ldr sensor!"));
    return 0;
  }else
  {
    Serial.println(ldrValue);
    return ldrValue;
  }

}