#include <IRremote.h>
#include <Wire.h>
#include "RTClib.h"

// --- CONFIGURATION DES PINS ---
const int IR_PIN    = A0; 
const int LDR_PIN   = A1;
const int BUZZER    = A2;
const int RED_PIN   = A3;
const int GREEN_PIN = 10;
const int BLUE_PIN  = 11;

RTC_DS3231 rtc;

// --- VARIABLES DE CONTRÔLE ---
int heureFixee = -1;    
String saisieIR = "";    
bool alarmeActive = false;
bool peutProgrammer = true;
bool alarmeDejaDeclenchee = false;
int lightThreshold = 600;

unsigned long dernierBip = 0;
bool buzzerAllume = false;
unsigned long dernierToggleBuzzer = 0;
bool etatBuzzer = false;
const int PERIODE_BUZZER = 500;

void setCouleur(int r, int g, int b) {
  digitalWrite(RED_PIN, r);
  digitalWrite(GREEN_PIN, g);
  digitalWrite(BLUE_PIN, b);
}

void buzzerTick() {
  if (buzzerAllume) {
    unsigned long now = micros();
    if (now - dernierToggleBuzzer >= PERIODE_BUZZER) {
      dernierToggleBuzzer = now;
      etatBuzzer = !etatBuzzer;
      digitalWrite(BUZZER, etatBuzzer);
    }
  } else {
    digitalWrite(BUZZER, LOW);
    etatBuzzer = false;
  }
}

void doReset() {
  heureFixee = -1;
  saisieIR = "";
  alarmeActive = false;
  alarmeDejaDeclenchee = false;
  peutProgrammer = true;
  buzzerAllume = false;
  digitalWrite(BUZZER, LOW);
  setCouleur(0, 0, 1);
  Serial.println("RESET - REPROGRAMMATION ACTIVEE");
}

void setup() {
  Serial.begin(9600);
  pinMode(BUZZER, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  
  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);
  
  if (!rtc.begin()) {
    Serial.println("Erreur: RTC non trouve!");
  }

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); 

  Serial.println("--- MEDBOX OPERATIONNELLE ---");
  setCouleur(0, 0, 1);
}

void loop() {
  DateTime maintenant = rtc.now();
  int heureActuelle = (maintenant.hour() * 100) + maintenant.minute();
  int valeurLumiere = analogRead(LDR_PIN);

  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 2000) {
    Serial.print("Heure RTC: ");
    if(maintenant.hour() < 10) Serial.print("0");
    Serial.print(maintenant.hour());
    Serial.print(":");
    if(maintenant.minute() < 10) Serial.print("0");
    Serial.println(maintenant.minute());
    lastPrint = millis();
  }

  // 2. LECTURE DE LA TÉLÉCOMMANDE
  if (IrReceiver.decode()) {
    if (IrReceiver.decodedIRData.protocol != 0 &&
        !(IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT)) {

      uint8_t cmd = IrReceiver.decodedIRData.command;
      Serial.print("CMD: 0x"); Serial.println(cmd, HEX);

      if (cmd == 0x45) {
        doReset();
      } else {
        int chiffre = translateIR(cmd);
        if (chiffre >= 0 && chiffre <= 9) {

          if (saisieIR.length() >= 4) {
            saisieIR = saisieIR.substring(saisieIR.length() - 3);
          }
          saisieIR += chiffre;

          Serial.print("Saisie: "); Serial.println(saisieIR);

          if (saisieIR.length() == 4) {
            if (saisieIR == "9999") {
              doReset();
            } else if (peutProgrammer) {
              heureFixee = saisieIR.toInt();
              saisieIR = "";
              peutProgrammer = false;
              Serial.print("ALARME PROGRAMMEE A: "); Serial.println(heureFixee);
              for(int i = 0; i < 200; i++) {
                digitalWrite(BUZZER, HIGH);
                delayMicroseconds(500);
                digitalWrite(BUZZER, LOW);
                delayMicroseconds(500);
              }
            } else {
              saisieIR = "";
            }
          }
        }
      }
    }
    IrReceiver.resume();
  }

  // 3. DÉCLENCHEMENT DE L'ALARME
  if (heureFixee > 0 && heureActuelle == heureFixee && !alarmeActive && !alarmeDejaDeclenchee) {
      alarmeActive = true;
      alarmeDejaDeclenchee = true;
      dernierBip = 0;
      buzzerAllume = false;
      Serial.println("!!! ALARME ACTIVE !!!");
  }

  if (alarmeDejaDeclenchee && heureActuelle != heureFixee) {
      alarmeDejaDeclenchee = false;
  }

  // 4. GESTION DES ÉTATS PHYSIQUES
  if (alarmeActive) {
      if (valeurLumiere > lightThreshold) {
          alarmeActive = false;
          buzzerAllume = false;
          peutProgrammer = true;
          saisieIR = "";
          digitalWrite(BUZZER, LOW);
          setCouleur(0, 1, 0);
          Serial.println("MEDICAMENT PRIS - ALARME CONSERVEE POUR DEMAIN");
      } else {
          unsigned long maintenant_ms = millis();
          if (maintenant_ms - dernierBip >= 150) {
              dernierBip = maintenant_ms;
              buzzerAllume = !buzzerAllume;
              if (buzzerAllume) {
                  setCouleur(1, 0, 0);
              } else {
                  setCouleur(0, 0, 0);
                  digitalWrite(BUZZER, LOW);
              }
          }
          buzzerTick();
      }
  } 
  else {
    if (heureFixee > 0 && valeurLumiere < lightThreshold) {
        setCouleur(0, 1, 0);
    } else {
        setCouleur(0, 0, 1);
    }
  }
}

int translateIR(uint8_t cmd) {
  switch (cmd) {
    case 0x16: return 0;
    case 0x0C: return 1;
    case 0x18: return 2;
    case 0x5E: return 3;
    case 0x08: return 4;
    case 0x1C: return 5;
    case 0x5A: return 6;
    case 0x42: return 7;
    case 0x52: return 8;
    case 0x4A: return 9;
    default: return -1;
  }
}