const int BUZZER = A2;

void setup() {
  pinMode(BUZZER, OUTPUT);
  Serial.begin(9600);
  Serial.println("Début du test du Buzzer sur A2...");
}

void loop() {
  // 1. Un bip court et sec (Test de connexion)
  Serial.println("Bip court...");
  tone(BUZZER, 1000); // Son à 1000 Hz
  delay(200);
  noTone(BUZZER);
  delay(800);

  // 2. Une sirène montante (Test de fréquence)
  Serial.println("Sirène montante...");
  for (int f = 500; f < 2000; f += 100) {
    tone(BUZZER, f);
    delay(50);
  }
  noTone(BUZZER);
  delay(1000);

  // 3. Trois bips rapides (Alerte Mariya)
  Serial.println("Test Alerte...");
  for (int i = 0; i < 3; i++) {
    tone(BUZZER, 2500);
    delay(100);
    noTone(BUZZER);
    delay(100);
  }
  delay(2000);
}