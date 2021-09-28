#ifndef _DHT_H
#define _DHT_H

#include "global.h"

void DHT_init() {
  //dht.begin(DHTPIN, DHTTYPE);
  dht.begin();
}

int getValuesDHT(float * t, float * h) {
  float _h = dht.readHumidity();
  float _t = dht.readTemperature();
  if(isnan(_h) || isnan(_t)) {
    Serial.println("Failed to read from DHT Sensor");
    return 0;
  }
  *h = _h;
  *t = _t;
  return 1;
}

#endif /* ifndef _DHT_H */
