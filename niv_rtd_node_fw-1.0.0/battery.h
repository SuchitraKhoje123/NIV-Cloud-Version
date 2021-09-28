#ifndef _BAT_H
#define _BAT_H

#include "global.h"

int getBattery() {
  digitalWrite(BATTERY_CS, HIGH);
  delay(200);
  float analogBat = analogRead(BATTERY_IN);
  Serial.println("Analog read battery_IN with BAttery_cs as HIGH:="); 
    
    Serial.println(analogBat); 
  delay(200);
  digitalWrite(BATTERY_CS, HIGH);
  return int(map(analogBat, 0, 4095, 0, 100));

}

#endif
