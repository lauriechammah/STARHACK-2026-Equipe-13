#include <Wire.h>        // Bibliothèque pour communiquer avec le module RTC
#include "RTClib.h"      // Bibliothèque pour l'horloge DS3231

RTC_DS3231 rtc;         // On crée l'objet pour parler à l'horloge

// Pin pour le buzzer et les LED
int buzzerPin = 8;
int redPin = 9;
int greenPin = 10;
int bluePin = 11;
int ldrPin = A0; // Photoresistance pour savoir si la boîte est ouverte

// Heure de l'alarme
int alarmHour = 14;    // 14h
int alarmMinute = 30;  // 14h30

int lightThreshold = 500; // Seuil lumière pour détecter ouverture (à régler selon ta boîte)

// Variables pour gérer l'alarme
bool alarmActive = false;    // Est-ce que l'alarme est en train de sonner ?
bool alarmDoneToday = false; // Est-ce que l'alarme a déjà sonné aujourd'hui ?
int lastDay = -1;            // Pour savoir quand réinitialiser l'alarme chaque jour

void setup() {
  // On dit à l'Arduino que certaines broches vont envoyer du courant
  pinMode(buzzerPin, OUTPUT); 
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  rtc.begin(); // On démarre l'horloge

  triggerAlarm(false); // Au début, LED bleu = attente
}

void loop() {
  DateTime now = rtc.now();       // On prend l'heure actuelle
  int ldrValue = analogRead(ldrPin); // On lit la lumière dans la boîte

  // Réinitialiser l'alarme chaque jour
  if(now.day() != lastDay){
    alarmDoneToday = false;       // L'alarme n'a pas encore sonné aujourd'hui
    lastDay = now.day();          // On mémorise le jour actuel
    triggerAlarm(false);          // LED bleu = attente
  }

  // Si c'est l'heure du médicament et que l'alarme n'a pas encore été déclenchée
  if(!alarmDoneToday && now.hour() == alarmHour && now.minute() == alarmMinute){
    alarmActive = true;           // On active l'alarme
  }

  // Si l'alarme est active et que la boîte est fermée (peu de lumière)
  if(alarmActive && ldrValue < lightThreshold){
    triggerAlarm(true);           // Bip + LED rouge
  } else {
    // Sinon on arrête l'alarme
    // Si la boîte est ouverte
    if(ldrValue >= lightThreshold && alarmActive){
      alarmActive = false;        // On arrête l'alarme
      alarmDoneToday = true;      // L'alarme est faite pour aujourd'hui
      stopAlarm(true);            // LED verte = médicament pris
    } else if(!alarmActive){
      stopAlarm(false);           // LED bleu = attente
    }
  }

  delay(100); // Petite pause pour pas que ça tourne trop vite
}

// triggerAlarm(bool active)
// active = true → on fait sonner le buzzer et LED rouge
// active = false → LED bleu = attente
void triggerAlarm(bool active){
  if(active){
    // On fait bip-bip
    digitalWrite(buzzerPin, HIGH); 
    delay(300);                     
    digitalWrite(buzzerPin, LOW);  
    delay(300);                     

    // LED rouge allumée, les autres éteintes
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);
  } else {
    // LED bleu = attente, buzzer éteint
    digitalWrite(buzzerPin, LOW);
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, HIGH);
  }
}

// stopAlarm(bool taken)
// taken = true → médicament pris → LED verte
// taken = false → LED bleu = attente
void stopAlarm(bool taken){
  digitalWrite(buzzerPin, LOW); // On éteint le buzzer
  digitalWrite(redPin, LOW);    // LED rouge éteinte
  if(taken){
    digitalWrite(greenPin, HIGH); // LED verte allumée
    digitalWrite(bluePin, LOW);
  } else {
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, HIGH);  // LED bleu = attente
  }
}