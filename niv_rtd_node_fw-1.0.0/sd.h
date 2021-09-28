#ifndef _LOG_H
#define _LOG_H

#include "global.h"
#include <SD.h>
#include <FS.h>

#define CS_PIN (5)
// 13 19 18 5

#define FILE_SAVE ("/readings")

int sdBegin() {
  if(!SD.begin(CS_PIN)) {
    Serial.println("Card Mount Failed");
    return 1;
  }
  Serial.println("SD Card mounted successfully.");
  return 0;
}

int readingsPresent() {
  if (SD.exists(FILE_SAVE)) {
    return 1;
  }
  return 0;
}

int writeReading(uint32_t tstamp, float temperature, float co2, float hum) {
  File ts;
  char tsr[40];
  if(readingsPresent()) {
    ts = SD.open(FILE_SAVE, FILE_APPEND);
  } else {
    ts = SD.open(FILE_SAVE, FILE_WRITE);
  }
  if ( !ts ) {
    Serial.println("Couldn't open File");
    return 0;
  }
  snprintf(tsr, 40, "%ld,%2.2f,%2.2f,%2.2f\n", tstamp, temperature, co2, hum);
  Serial.println(tsr);

  int bytes_written = ts.print(tsr);
	if(!bytes_written) {
		Serial.println("Timestamp Write Failed");
	}
	Serial.print(bytes_written);
	Serial.println(" bytes were written.");
	Serial.println("Closing file /readings");
	ts.close();
}

int clearFile(const char * file) {
  SD.remove(file);
}

String readAllTimestamps() {
  String fmt = "";
	File ts = SD.open(FILE_SAVE, FILE_READ);
	if(!ts) {
		Serial.println("File Not Found.");
		return "";
	}
	while(ts.available()) {
    fmt += ts.read();
	}
  ts.close();
  clearFile(FILE_SAVE);
	return fmt;
}
#endif
