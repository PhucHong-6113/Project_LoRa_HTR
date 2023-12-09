#include "SevSeg.h"
SevSeg sevseg;
#include <Wire.h>
#include <DS3231.h>
DS3231 clock;
RTCDateTime dt;

unsigned long lastBlink = 0;
const int blinkInterval = 1000; // Blink interval in milliseconds

void setup() {
  Serial.begin(9600);
  byte numDigits = 4;
  const byte digits[]={7,4,3,8};// define digits pins
  const byte displayPins[] = {6,2,10,12,11,5,9,13}; // define segment pins
  bool resistorsOnSegments = 0;
  clock.begin();//for DS3231
  sevseg.begin(COMMON_CATHODE, numDigits, digits, displayPins, resistorsOnSegments);
  sevseg.setBrightness(60);
}

void loop() {
  dt = clock.getDateTime();

  // Convert hour and minute to strings and char arrays
  String h = (String)dt.hour;
  char hChar[h.length() + 1];
  h.toCharArray(hChar, sizeof(hChar));
  String m = (String)dt.minute;
  if(m.length() <2 ){ m = '0' + m;}
  Serial.print(m);
  char mChar[m.length() + 1];
  m.toCharArray(mChar, sizeof(mChar));

  // Combine hour and minute into a single char array for display
  char rTime[] = {h[0], h[1], m[0], m[1], '\0'};

  // Blink logic
  if (millis() - lastBlink >= blinkInterval) {
    sevseg.setBrightness(0); // Turn off all segments
    lastBlink = millis();
  } else if (millis() - lastBlink >= blinkInterval / 2) {
    sevseg.setBrightness(60); // Turn on all segments
  }

  // Set the characters to display and refresh the display
  sevseg.setChars(rTime);
  sevseg.refreshDisplay();
}
