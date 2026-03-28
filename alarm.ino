#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

int buzzerPin = 8;
int redPin = 9;
int greenPin = 10;
int bluePin = 11;
int ldrPin = A0; // Photoresistance pour détecter l'ouverture de la boîte

int alarmHour = 14;    // Heure du rappel (à modifier selon besoin)
int alarmMinute = 30;

int lightThreshold = 500; // Seuil pour détecter boîte ouverte (ajuster selon luminosité)

bool alarmActive = false;
bool alarmDoneToday = false;
int lastDay = -1;

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  rtc.begin();

  // LED bleu = état attente au démarrage
  digitalWrite(bluePin, HIGH);
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
}

void loop() {
  DateTime now = rtc.now();
  int ldrValue = analogRead(ldrPin);

  // Réinitialiser l'alarme chaque jour
  if(now.day() != lastDay){
    alarmDoneToday = false;
    lastDay = now.day();

    // LED bleu = attente
    digitalWrite(bluePin, HIGH);
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
  }

  // Déclenchement de l'alarme si pas encore faite aujourd'hui et à l'heure
  if(!alarmDoneToday && now.hour() == alarmHour && now.minute() == alarmMinute){
    alarmActive = true;
    digitalWrite(bluePin, LOW); // Arrête état attente
  }

  // Si alarme active et boîte fermée (peu de lumière)
  if(alarmActive && ldrValue < lightThreshold){
    triggerAlarm();
  } else {
    stopAlarm();
    // Si boîte ouverte (plus de lumière)
    if(ldrValue >= lightThreshold){
      alarmActive = false;
      alarmDoneToday = true; // Rappel fait pour aujourd'hui
      digitalWrite(greenPin, HIGH); // LED verte = médicament pris
      digitalWrite(redPin, LOW);
      digitalWrite(bluePin, LOW);
    }
  }

  delay(100);
}

void triggerAlarm() {
  // Bip intermittent
  digitalWrite(buzzerPin, HIGH);
  delay(300);
  digitalWrite(buzzerPin, LOW);
  delay(300);

  // LED rouge = alerte oubli
  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);
}

void stopAlarm() {
  digitalWrite(buzzerPin, LOW);
  digitalWrite(redPin, LOW);
  // LED bleu reste gérée dans loop()
}