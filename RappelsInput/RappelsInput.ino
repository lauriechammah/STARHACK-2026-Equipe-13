#include <IRremote.h>
#include <SevSeg.h>
#include <RTClib.h>

const int RECEIVE_PIN = 2; // pourquoi
int reminder = 0;
SevSeg sevseg;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // vitesse de transmission en bits par seconde (default one pour UNO R3)

  IrReceiver.begin(RECEIVE_PIN);

  byte numDigits = 4;
  byte digitPins[] = {6, 12, A2, A3};
  byte segmentPins[] = {3, 4, 5, 7,13, A1, 0, 1};

  sevseg.begin(COMMON_ANODE, numDigits, digitPins, segmentPins);
  sevseg.setBrightness(90);
}

int translateIR() {
  switch (IrReceiver.decodedIRData.decodedRawData) {
    case 0xFF6897: return 0;
    case 0xFF30CF: return 1;
    case 0xFF18E7: return 2;
    case 0xFF7A85: return 3;
    case 0xFF10EF: return 4;
    case 0xFF38C7: return 5;
    case 0xFF5AA5: return 6;
    case 0xFF42BD: return 7;
    case 0xFF4AB5: return 8;
    case 0xFF52AD: return 9;
    default: return -1;
  }
}

int readHour() {
  String timeInput = "";
  while (timeInput.length() < 4) {
    if (IrReceiver.decode()) {
      int val = translateIR();
      if (val >= 0 && val <= 9) {
        timeInput += val;
        sevseg.setNumber(timeInput.toInt(), 0);
      }
      IrReceiver.resume();
    }
    sevseg.refreshDisplay();
  }
  return timeInput.toInt();
}
void loop() {
  // put your main code here, to run repeatedly:
  if (reminder == 0){
    reminder = readHour();
    Serial.println("Heure fixée : ");
    Serial.println(reminder);
  }
  sevseg.setNumber(reminder, 0);
  sevseg.refreshDisplay();
}
