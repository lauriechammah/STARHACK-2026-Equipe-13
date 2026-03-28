#include <Keypad.h>

const byte ROWS = 4;
const byte COLUMNS = 4;

char keys[ROWS][COLUMNS] = {
  {'1','2','3','A'},
  {'4','5','6','B'}, 
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {2, 3, 4, 5};
byte columnPins[COLUMNS] = {6, 7, 8, 9};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, ROWS, COLUMNS);

int reminder = 0;

int readHour() {
  String timeInput = "";
  char key;

  Serial.println("Veuillez entrer l'heure de rappel (ex : 0900) suivie d'un # : ");

  while(true) {
    key = keypad.getKey();

    if (key >= '0' && key <= '9') {
      if (timeInput.length() < 4) {
        timeInput += key;
        Serial.print(key);
      }
    }
    else if (key == '#') { // Enter reminder time
      if (timeInput.length() == 4) {
        int result = timeInput.toInt();
        Serial.println("\nHeure de rappel : " + timeInput);
        return result;
      }
       else {
      Serial.println("\nHeure invalide. Veuillez recommencer : ");
      }
    }
    else if (key == 'C') { // clear if mistakes
      userInput = "";
      Serial.println("\nHeure effacée. Veuillez réessayer : ");
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // vitesse de transmission en bits par seconde (default one pour UNO R3)
  Serial.println("----SMART-MED-BOX DÉMARRÉE----");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (reminder == 0){
    reminder = readHour();
    Serial.println("Heure fixée : ")
    Serial.println(reminder);
  }

}
