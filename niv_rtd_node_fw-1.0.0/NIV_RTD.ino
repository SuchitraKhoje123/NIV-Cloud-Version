
#include "global.h"
#include "sd.h"
#include "rtc.h"
#include "wifi.h"
#include "data.h"
#include "routes.h"
#include "mem.h"
#include "lcd.h"
#include "battery.h"
#include "setpoints.h"
/* #include "deep_sleep.h" */
#define RTD_PIN 25
#ifdef CO2_NODE
#include "mh_z16_co2.h"
#endif

#ifdef DHT_NODE
#include "dht.h"
float dht_temp, dht_hum;
#endif

#ifdef RTD_NODE
#include "temperature.h"
#define RREF 430.0
#define RNOMINAL 100.0
uint32_t rtd;
float temperature;
#endif

#ifdef CO2_NODE
float co2;
#endif

/* #define DEBUG */

int battery = 0;
DateTime n;

void setup() {
  Serial.begin(115200);
#ifdef RTD_NODE
  thermo.begin(MAX31865_3WIRE);
#endif
  EEPROM.begin(512);
  lcdSetup();
  uint8_t err = 0;

  err += rtcBegin();
  err += sdBegin();

#ifdef CO2_NODE
  co2Sensor.begin(UART2_RX, UART2_TX);
#endif

  checkWifi(1);
  Serial.println();
  if(err) {
    Serial.println("There was an error");
#ifndef DEBUG
    while(1);
#endif
  }

#ifdef DHT_NODE
  DHT_init();
#endif

  server.begin();
  server.on("/", handleRoot);
  server.on("/save", handleSave);
  server.onNotFound(handleNotFound);

  checkEEPROM();
  Serial.println("=====EEPROM VALUES======");
  Serial.print("SSID: ");
  Serial.println(SSID);
  Serial.print("PASS: ");
  Serial.println(PASS);
  Serial.print("UID: ");
  Serial.println(UID);
  Serial.print("USER: ");
  Serial.println(USER);
  Serial.print("HOST: ");
  Serial.println(HOST);
  Serial.println("=======================");
  Serial.println();
  Serial.println(WiFi.localIP());

  if(!MDNS.begin("esp")) {
    Serial.println("Error starting mDNS");
    return;
  }

  /* esp_sleep_enable_timer_wakeup(TS * uS_to_S); */
  /* Serial.println("Setup ESP32 to sleep for every " + String(TS) + */
  /* " Seconds"); */
  /* esp_deep_sleep_start(); */
  delay(20000);
#ifdef RTD_NODE
  rtd = thermo.readRTD();
#endif
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RTD_PIN, OUTPUT);


  pinMode(BATTERY_CS, OUTPUT);
  pinMode(BATTERY_IN, INPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // Load setpoints

}

void loop() {
    setvalues=fetchSetpoint();
    digitalWrite(RTD_PIN, HIGH);
    TEMP_MIN=setvalues[0] ;
    TEMP_MAX=setvalues[1] ;
    HUM_MIN=setvalues[2] ;
    HUM_MAX=setvalues[3] ;
    CO2_MIN=setvalues[4] ;
    CO2_MAX=setvalues[5] ;
 /* HTTPClient http;

  String url = PROT + HOST + "/write/setpoints/" + UID; http.begin(url);
  http.addHeader("Accept", "application/json");

  Serial.println(url);
  int resp = http.GET();
  String response = http.getString();
  Serial.println(response);

  DynamicJsonDocument json(1024);
  deserializeJson(json, response);

  float TEMP_MIN=json["temperaturemin"];
  float TEMP_MAX=json["temperaturemax"];
  http.end();*/
  //Serial.println(TEMP_MIN);
 // Serial.println(TEMP_MAX);
  //+ " " + TEMP_MAX);
  //Serial.println(HUM_MIN + " " + HUM_MAX);
  //Serial.println(CO2_MIN + " " + CO2_MAX);
  server.handleClient();
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  checkWifi(0);

  Serial.println();
#ifdef RTD_NODE

  temperature = thermo.temperature(RNOMINAL, RREF);
  Serial.print("RTD temperature: ");
  Serial.println(temperature);
  if(temperature > TEMP_MAX || temperature < TEMP_MIN) {
    digitalWrite(BUZZER_PIN, HIGH);
    } else {
      digitalWrite(BUZZER_PIN, LOW);
    }
#endif

#ifdef CO2_NODE
  if(!co2Sensor.isWarming()) {
    co2 = co2Sensor.getPPM();
    if(co2 > CO2_MAX || co2 < CO2_MIN) {
      digitalWrite(BUZZER_PIN, HIGH);
    } else {
      digitalWrite(BUZZER_PIN, LOW);
    }
  } else {
    Serial.println("Error: Sensor Communication Error in CO2");
  }
#endif

#ifdef RTD_NODE
  Serial.println(temperature);
  checkFault();
  Serial.println();
#endif

  displayUpdate(
      // Temperature and Humidity
#if defined(RTD_NODE)
      temperature,
      0,
#elif defined(DHT_NODE)
      dht_temp,
      dht_hum,
#elif defined(CO2_NODE)
      0,
      0,
#endif

      // CO2
#if defined(CO2_NODE)
      co2,
#else
      0,
#endif


      battery=getBattery()
      );

  // Battery Reading
  battery = getBattery();

#ifdef DHT_NODE
  getValuesDHT(&dht_temp, &dht_hum);

 /* HTTPClient http;

  String url = PROT + HOST + "/write/setpoints/" + UID; http.begin(url);
  http.addHeader("Accept", "application/json");

  Serial.println(url);
  int resp = http.GET();
  String response = http.getString();
  Serial.println(response);

  DynamicJsonDocument json(1024);
  deserializeJson(json, response);

  float TEMP_MIN=json["temperaturemin"];
  float TEMP_MAX=json["temperaturemax"];
  float HUM_MIN=json["humiditymin"];
  float HUM_MAX=json["humiditymax"];
  http.end();*/
  //TEMP_MIN=fetchSetpoint();
  Serial.println("Current Temp:=");
  Serial.println(dht_temp);
  Serial.println("Min. Temp:=");
  Serial.println(TEMP_MIN);
  Serial.println("Max. Temp:=");
  Serial.println(TEMP_MAX);
  int condition=(dht_temp > TEMP_MAX || dht_temp < TEMP_MIN);
     Serial.println(condition);

     Serial.println("Current Hum=");
  Serial.println(dht_hum);
  Serial.println("Min. hum:=");
  Serial.println(HUM_MIN);
  Serial.println("Max. hum:=");
  Serial.println(HUM_MAX);
   int condition1=((dht_hum > HUM_MAX) || (dht_hum < HUM_MIN));
   Serial.println(condition1);
   digitalWrite(BUZZER_PIN, LOW);
   Serial.println(BUZZER_PIN);
  if((dht_temp > TEMP_MAX) || (dht_temp < TEMP_MIN)) {
    temp_flag=1;
  } else {
    temp_flag=0;
  }
  if((dht_hum > HUM_MAX) || (dht_hum < HUM_MIN)) {
   hum_flag=1;
 } else {
   hum_flag=0;
  }
 if(temp_flag==1 || hum_flag==1||RTD_flag==1) {
      digitalWrite(BUZZER_PIN, HIGH);
    } else {
      digitalWrite(BUZZER_PIN, LOW);
    }
    Serial.println("battery:=");
    battery = getBattery();
    Serial.println(battery);
#endif

  n = getTime();
  uint32_t ts = 0;
#ifdef DEBUG
  ts = 1623718150;
#else
  ts = n.unixtime();
#endif


  storeData(ts,
#ifdef DHT_NODE
      dht_temp
#else
      temperature
#endif
      ,
#ifdef CO2_NODE
      co2
#else
      0.0
#endif
      ,
#ifdef DHT_NODE
      dht_hum
#else
      0.0
#endif
      ,
      battery
      );


  delay(TS * 1000);
}
