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
bool dejaPrisAujourdhui = false; // NOUVEAU : Pour ne pas sonner 2 fois le même jour
int dernierJourCheck = -1;       // Pour savoir quand on change de jour
int lightThreshold = 600; 

void setCouleur(int r, int g, int b) {
  digitalWrite(RED_PIN, r);
  digitalWrite(GREEN_PIN, g);
  digitalWrite(BLUE_PIN, b);
}

void setup() {
  Serial.begin(9600);
  pinMode(BUZZER, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  
  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);
  rtc.begin();
  
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // À commenter après le 1er test
}

void loop() {
  DateTime maintenant = rtc.now();
  int heureActuelle = (maintenant.hour() * 100) + maintenant.minute();
  int valeurLumiere = analogRead(LDR_PIN);

  // 1. RÉINITIALISATION QUOTIDIENNE (À minuit)
  if (maintenant.day() != dernierJourCheck) {
      dejaPrisAujourdhui = false; 
      dernierJourCheck = maintenant.day();
      Serial.println("Nouveau jour : Prise autorisee");
  }

  // 2. LECTURE DE LA TÉLÉCOMMANDE
  if (IrReceiver.decode()) {
    unsigned long code = IrReceiver.decodedIRData.decodedRawData;
    
    if (code == 0xBA45FF00) { // Bouton POWER = RESET / REPROGRAMMATION
        heureFixee = -1;
        saisieIR = "";
        alarmeActive = false;
        dejaPrisAujourdhui = false;
        noTone(BUZZER);
        Serial.println("MODE REPROGRAMMATION ACTIVE");
        setCouleur(0, 0, 1); // Bleu pour dire qu'on attend une saisie
    } else {
        int chiffre = translateIR(code);
        if (chiffre >= 0 && chiffre <= 9 && heureFixee == -1) {
            saisieIR += chiffre;
            if (saisieIR.length() == 4) {
                heureFixee = saisieIR.toInt();
                Serial.print("NOUVELLE HEURE FIXEE : "); Serial.println(heureFixee);
                tone(BUZZER, 1000, 200); 
            }
        }
    }
    IrReceiver.resume();
  }

  // 3. DÉCLENCHEMENT DE L'ALARME
  // On ne déclenche que si l'heure concorde ET qu'il n'a pas encore pris son med
  if (heureFixee > 0 && heureActuelle == heureFixee && !alarmeActive && !dejaPrisAujourdhui) {
      alarmeActive = true;
      Serial.println("!!! ALARME ACTIVE !!!");
  }

  // 4. GESTION DE LA PRISE
  if (alarmeActive) {
      if (valeurLumiere > lightThreshold) { // Boîte ouverte
          alarmeActive = false;
          dejaPrisAujourdhui = true; // ON GARDE L'HEURE, mais on dit que c'est fait
          noTone(BUZZER);
          Serial.println("MEDICAMENT PRIS - HEURE MEMORISEE POUR DEMAIN");
      } else {
          // Alerte visuelle et sonore
          setCouleur(1, 0, 0); 
          tone(BUZZER, 2000, 150);
          delay(300);
          setCouleur(0, 0, 0);
          delay(150);
      }
  } 
  else {
    // États de la LED
    if (dejaPrisAujourdhui) setCouleur(0, 1, 0); // VERT : C'est fait pour aujourd'hui
    else if (heureFixee > 0) setCouleur(1, 1, 0); // JAUNE/ORANGE : En attente de l'heure
    else setCouleur(0, 0, 1); // BLEU : Pas encore programmé
  }
}

int translateIR(unsigned long raw) {
  switch (raw) {
    case 0xE916FF00: return 0;
    case 0xF30CFF00: return 1;
    case 0xE718FF00: return 2;
    case 0xA15EFF00: return 3;
    case 0xF708FF00: return 4;
    case 0xE31CFF00: return 5;
    case 0xA55AFF00: return 6;
    case 0xBD42FF00: return 7;
    case 0xAD52FF00: return 8;
    case 0xB54AFF00: return 9;
    default: return -1;
  }
}