#include <SevSeg.h>
#include <RTClib.h>

SevSeg sevseg;

int lastTakenHour = 0;
bool complianceStatus = true; // Statut de conformité de Florence

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("--- SMART-MED-BOX INITIALISÉE---");

  byte numDigits = 4;

  byte digitPins[] = {12, 6, A2, A3};

  byte segmentPins[] = {2, 3, 4, 5, 7, 13, A1, 1};

  sevseg.begin(COMMON_ANODE, numDigits, digitPins, segmentPins);
  sevseg.setBrightness(90);
}

void updateDisplay(int medTime, DateTime now, bool isDanger) {
  if (isDanger) {
    sevseg.setChars("ERR");
  }
  else {
    int timeToDisplay = (now.hour() * 100) + now.minute();
    sevseg.setNumber(timeToDisplay, 0);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  sevseg.refreshDisplay();
}
