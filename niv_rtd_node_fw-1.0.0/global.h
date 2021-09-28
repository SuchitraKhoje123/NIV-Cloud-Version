#ifndef _GLOBAL_H
#define _GLOBAL_H
#include <Adafruit_MAX31865.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <WebServer.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <Wire.h>
#include <MHZ16_uart.h>
#include <DHT.h>
#include <ESPmDNS.h>

#include "ArduinoJson-v6.18.3.h"

#define RTD_NODE
//#define DHT_NODE
#define CO2_NODE

#define DEF_UID "000000"
#define DEF_USER "kaustubh"

#define DEF_HOST "192.168.77.127:3000"
//#define DEF_SSID ("Kaustubh_Pi")
//#define DEF_PASS ("kaustubhPi")
#define DEF_SSID ("AndroidAP9199")
#define DEF_PASS ("Psk101104^")
//#define DEF_SSID ("sanjay")
//#define DEF_PASS ("sanjay1969")
//#define DEF_SSID ("ATECHNO")
//#define DEF_PASS ("Sanjay1969")

#define PROT "http://"
#define ENDPOINT "/write/reading"

#define uS_to_S (1000000)

#define DHTTYPE DHT11
#define DHTPIN (4)

#define BATTERY_IN (36) // or 25
#define BATTERY_CS (26)

#define BUZZER_PIN (27)
//#define BUZZER_PIN (33)

WebServer server(80);
LiquidCrystal_I2C lcd(0x27, 20, 4);
String SSID, PASS;
String HOST, UID, USER;
Adafruit_MAX31865 thermo = Adafruit_MAX31865(27, 14, 12, 13);
//Adafruit_MAX31865 thermo = Adafruit_MAX31865(CS, SDI, SDO, clk);
//Adafruit_MAX31865 thermo = Adafruit_MAX31865(15, 13, 12, 14);
DHT dht(DHTPIN, DHTTYPE);
uint16_t TS;
float TEMP_MIN, TEMP_MAX;
float HUM_MIN, HUM_MAX;
float CO2_MIN, CO2_MAX;
float *setvalues;
int temp_flag=0, hum_flag=0, RTD_flag=0, CO2_flag=0;
#endif
