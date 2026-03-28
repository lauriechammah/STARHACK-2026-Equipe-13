const int RED = A3;
const int GREEN = 10;
const int BLUE = 11;

void setup()
{
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);
    Serial.begin(9600);
    Serial.println("Début du test des couleurs...");
}

void loop()
{
    // 1. ROUGE pur
    Serial.println("Rouge");
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);
    delay(1500);

    // 2. VERT pur (C'est ici qu'on vérifie si ta Pin 10 fonctionne !)
    Serial.println("Vert");
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, HIGH);
    digitalWrite(BLUE, LOW);
    delay(1500);

    // 3. BLEU pur
    Serial.println("Bleu");
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, HIGH);
    delay(1500);

    // 4. JAUNE (Mélange Rouge + Vert)
    Serial.println("Jaune (Test du mélange)");
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, HIGH);
    digitalWrite(BLUE, LOW);
    delay(1500);

    // 5. BLANC (Tout allumé)
    Serial.println("Blanc (Toutes les pins)");
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, HIGH);
    digitalWrite(BLUE, HIGH);
    delay(1500);

    // Éteindre avant de recommencer
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);
    delay(1000);
}