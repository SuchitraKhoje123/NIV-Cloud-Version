#ifndef _DATA_H
#define _DATA_H

#include "global.h"

int postRequestWithTS(float t, float co2, float hum, uint32_t ts) {

  char postdata[200];
  HTTPClient http;
  http.begin(PROT + HOST + ENDPOINT);
  Serial.println(PROT + HOST + ENDPOINT);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Accept", "application/json");

  snprintf(postdata, 200,
      "{"
      "\"uid\":\"%s\","
      "\"user\":\"%s\","
      "\"temperature\":\"%0.2f\","
      "\"humidity\":\"%0.2f\","
      "\"co2\":\"%0.2f\","
      //"\"pressure\":\"0\","
      "\"backup\": \"1\","
      "\"datetime\": \"%ld\""
      "}",
      UID,
      USER,
      t,
      hum,
      co2,
      ts
      );

  Serial.println(postdata);

  int resp = http.POST(postdata);
  Serial.println(resp);
  Serial.println(http.getString());
  http.end();
  return resp;
}

int postRequest(float t, float co2, float hum, int bat) {

  char postdata[200];
  HTTPClient http;
  http.begin(PROT + HOST + ENDPOINT);
  Serial.println(PROT + HOST + ENDPOINT);
  http.addHeader("Content-Type", "application/json");

  snprintf(postdata, 200,
      "{\"uid\":\"%s\","
      "\"user\":\"%s\","
      "\"temperature\":\"%0.2f\","
      "\"humidity\":\"%0.2f\","
      "\"co2\":\"%0.2f\","
      "\"pressure\":\"0\","
      "\"battery\":\"%d\"}",
      UID,
      USER,
      t,
      hum,
      co2,
      //bat%100
      bat
      );

  Serial.println(postdata);

  int resp = http.POST(postdata);
  Serial.println(http.getString());
  http.end();
  return resp;
}

uint8_t storeData(uint32_t ts, float temperature, float co2, float hum, float battery) {
  if(WiFi.status() == WL_CONNECTED) {
    if(readingsPresent()) {
      Serial.println("Readings Present");
      File ts = SD.open(FILE_SAVE, FILE_READ);
      String fmt = "";
      if(!ts) {
        Serial.println("File not found.");
        return 0;
      }
      while(ts.available()) {
        Serial.println(ts.available());
        String f = ts.readStringUntil('\n');
        if(f.length() < 4) {
          Serial.println("Readings Empty");
          break;
        }
        uint32_t tstamp;
        float tem, t_co2, t_hum;
        sscanf(f.c_str(), "%ld,%f,%f,%f", &tstamp, &tem, &t_co2, &t_hum);
        postRequestWithTS(tem, t_co2, t_hum, tstamp);
      }
      ts.close();
      clearFile(FILE_SAVE);
      Serial.println("Sent SD card data");

    } else {
      int resp = postRequest(temperature, co2, hum, battery);
      Serial.println("Sent live data");
      return resp;
    }
  } else {
    writeReading(ts, temperature, co2, hum);
    Serial.println("Saved data to SD card");
    return 1;
  }
}

#endif
