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
int lightThreshold = 600; // Calibré selon tes tests (300-400 noir, 800+ lumière)

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
  
  if (!rtc.begin()) {
    Serial.println("Erreur: RTC non trouve!");
  }

  // --- SYNCHRONISATION DE L'HEURE ---
  // Cette ligne prend l'heure de ton ordi AU MOMENT DU TELEVERSEMENT
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); 

  Serial.println("--- MARIYA OPERATIONNELLE ---");
  setCouleur(0, 0, 1); // Démarrage en BLEU
}

void loop() {
  // 1. RÉCUPÉRATION DE L'HEURE ET DE LA LUMIÈRE
  DateTime maintenant = rtc.now();
  int heureActuelle = (maintenant.hour() * 100) + maintenant.minute();
  int valeurLumiere = analogRead(LDR_PIN);

  // Affichage de l'heure toutes les 2 secondes pour vérifier le décalage
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
    unsigned long code = IrReceiver.decodedIRData.decodedRawData;
    
    if (code == 0xBA45FF00) { // Bouton POWER = RESET manuel
        heureFixee = -1;
        saisieIR = "";
        alarmeActive = false;
        noTone(BUZZER);
        Serial.println("RESET MANUEL");
    } else {
        int chiffre = translateIR(code);
        if (chiffre >= 0 && chiffre <= 9 && !alarmeActive) {
            saisieIR += chiffre;
            Serial.print("Chiffre recu: "); Serial.println(chiffre);
            if (saisieIR.length() == 4) {
                heureFixee = saisieIR.toInt();
                Serial.print("ALARME PROGRAMMEE A: "); Serial.println(heureFixee);
                tone(BUZZER, 1000, 200); 
            }
        }
    }
    IrReceiver.resume();
  }

  // 3. DÉCLENCHEMENT DE L'ALARME
  if (heureFixee > 0 && heureActuelle == heureFixee && !alarmeActive) {
      alarmeActive = true;
      Serial.println("!!! ALARME ACTIVE !!!");
  }

  // 4. GESTION DES ÉTATS PHYSIQUES
  if (alarmeActive) {
      // Si lumière détectée (boîte ouverte)
      if (valeurLumiere > lightThreshold) {
          alarmeActive = false;
          heureFixee = -1; // Efface l'alarme pour la suivante
          saisieIR = "";
          noTone(BUZZER);
          Serial.println("MEDICAMENT PRIS - ALARME STOPPEE");
      } else {
          // Bip Rouge clignotant
          setCouleur(1, 0, 0); 
          tone(BUZZER, 2000);
          delay(150);
          setCouleur(0, 0, 0);
          noTone(BUZZER);
          delay(150);
      }
  } 
  else {
    // Mode veille / Programmation
    if (heureFixee > 0 && valeurLumiere < lightThreshold) {
        setCouleur(0, 1, 0); // VERT : Alarme prête + Boîte fermée
    } else {
        setCouleur(0, 0, 1); // BLEU : En attente ou Boîte ouverte
    }
  }
}

// --- FONCTION DE TRADUCTION IR ---
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