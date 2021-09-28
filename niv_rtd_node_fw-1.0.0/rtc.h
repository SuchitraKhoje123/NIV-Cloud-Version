#ifndef _RTC_A_H
#define _RTC_A_H

#include <RTClib.h>

RTC_DS3231 rtc;
DateTime now;

uint8_t rtcBegin() {
  if(!rtc.begin()) {
    Serial.println("Could Not Find RTC");
    return 1;
  }

  if(rtc.lostPower()) {
    Serial.println("RTC has lost power.");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  Serial.println("RTC initiated successfully.");
  return 0;
}

DateTime getTime() {
  return rtc.now();
}



#endif
