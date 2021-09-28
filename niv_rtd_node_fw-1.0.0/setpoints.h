#ifndef _SETPT_H
#define _SETPT_H

#include "global.h"

  float * fetchSetpoint(void) {
  HTTPClient http;

  String url = PROT + HOST + "/write/setpoints/" + UID;
  http.begin(url);
  http.addHeader("Accept", "application/json");

  Serial.println(url);
  int resp = http.GET();
  String response = http.getString();
  Serial.println(response);
 
  DynamicJsonDocument json(1024);
  deserializeJson(json, response);

  float TEMP_MIN = json["temperaturemin"];
  float TEMP_MAX = json["temperaturemax"];
  float HUM_MIN = json["humiditymin"];
  float HUM_MAX = json["humiditymax"];
  float CO2_MIN = json["co2min"];
  float CO2_MAX = json["co2max"];

  //String ddata;
  //ddata = json['temperaturemin'].as<const char*>();
  //ddata += ", ";
  //ddata += json['temperaturemax'].as<const char*>();
  //ddata += "\n";
  //ddata += json['humiditymin'].as<const char*>();
  //ddata += ", ";
  //ddata += json['humiditymax'].as<const char*>();
  //ddata += "\n";
  //ddata += json['co2min'].as<const char*>();
  //ddata += ", ";
  //ddata += json['co2max'].as<const char*>();
  // ddata += "\n";

  Serial.println("+++++++++JSON DATA++++++++++");
  Serial.println("Min. Temp:=");
  Serial.println(TEMP_MIN);
  Serial.println("Max. Temp:=");
  Serial.println(TEMP_MAX);
  Serial.println("Min. HUM:=");
  Serial.println(HUM_MIN);
  Serial.println("Max. HUM:=");
  Serial.println(HUM_MAX);
  Serial.println("++++++++++++++++++++++++++++");

  updateSetPoint(TEMP_MIN_ADDR, json['temperaturemin'], json['temperaturemax']);
  // updateSetPoint(TEMP_MIN_ADDR,20,30);
  updateSetPoint(HUM_MIN_ADDR, json['humiditymin'], json['humiditymax']);
  updateSetPoint(CO2_MIN_ADDR, json['co2min'], json['co2max']);

  http.end();
  static float setvalues[6];
    setvalues[0] = TEMP_MIN; 
    setvalues[1] = TEMP_MAX; 
    setvalues[2] = HUM_MIN;
    setvalues[3] = HUM_MAX; 
    setvalues[4] = CO2_MIN; 
    setvalues[5] = CO2_MAX; 
    return setvalues;

}

#endif
