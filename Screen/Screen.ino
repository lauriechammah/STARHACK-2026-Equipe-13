#include <LiquidCrystal_I2C.h>
#include <RTClib.h>

LiquidCrystal_I2C lcd(0x27, 16, 2) // Adresse puce, nb de colonnes, nb de lignes

void initializeScreen() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void updateScreen(int medTime, DateTime now, bool isDanger) {
  // Rappel programmé
  lcd.setCursor(0,0);
  lcd.print("Rappel: ");
  if(medTime < 1000) {
    lcd.print("0");
  }
  lcd.print(medTime / 100);
  lcd.print(":");
  if ((medTime % 100) < 10) { // séparer heures et minutes
    lcd.print("0");
  }
  lcd.print(medTime % 100);
  
  // Heure actuelle et remaining time
  lcd.setCursor(0,1);
  if (isDanger) {
    lcd.print("PRENDRE MED!!!");
  }
  else {
    if(now.hour() < 10) {
      lcd.print("0");
    }
    lcd.print(now.hour());
    lcd.print(":");
    if (now.minute() < 10) {
      lcd.print("0");
    }
    lcd.print(now.minute());
    lcd.print("C'est OK!");
  }
}
void setup() {
  // put your setup code here, to run once:
  initializeScreen();
}

void loop() {
  // put your main code here, to run repeatedly:

}
