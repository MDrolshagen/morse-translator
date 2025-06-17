#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Adresse des LCD-Displays

//Deklaration von Varbablen
//=========================
const int buttonPin = 18;
const int linePin = 19;
const int setPin = 23;


int morseType = 0;
int textType = 0;

int BtnStart;
int BtnEnd;
int BtnDauer;


String morseCode = "";
String currentCharacter = "";
String plainText = "";

bool pressedB = false;
bool pressedA = false;
bool pressedC = false;


void setup() { //Wird am Anfang des Programs ausgeführt
  Wire.begin(21,22);
  lcd.init();
  lcd.clear();
  lcd.noBacklight();
  lcd.begin(16, 2);
  Serial.begin(9600);
  

  BtnStart = 0;
  pinMode(buttonPin, INPUT);
  pinMode(linePin, INPUT);
  
}

void loop() { //Wird fortlaufend wiederholt

  
  handleInput();
  Translate();

  lcd.setCursor(textType,0);
  lcd.print(currentCharacter);
  
}

void handleInput() { //Verabreitet die Eingaben
  
    if ((digitalRead(buttonPin) == HIGH) && (pressedB == false)) { // Taster S4 gedrückt
      BtnStart = millis();
      pressedB = true;
      delay(20);
    }
  
    if ((digitalRead(buttonPin) == LOW) && (pressedB == true)){
      BtnEnd = millis();
      BtnDauer = BtnEnd - BtnStart;
      if (BtnDauer > 50) { // Lange gedrückt
        morseCode += "-";
        Ausgabe();
        Serial.print(morseCode);
        morseType++;
      } else {
        lcd.print("0");
    }
      pressedB = false;
      delay(20);
      }
    if ((digitalRead(buttonPin) == HIGH) && (pressedA == false)) { // Taster S3 gedrückt
      BtnStart = millis();
      pressedA = true;
      delay(20);
    }
      if ((digitalRead(linePin) == LOW) && (pressedA == true)){
      BtnEnd = millis();
      BtnDauer = BtnEnd - BtnStart;
      if (BtnDauer > 50) { // Lange gedrückt
        morseCode += ".";
        Ausgabe();
        Serial.print(morseCode);
        morseType++;
      } 
      pressedA = false;
      delay(20);
      }
  
        if ((digitalRead(setPin) == HIGH) && (pressedC == false)) { // Taster S2 gedrückt
      BtnStart = millis();
      pressedC = true;
      delay(20);
    }
      if ((digitalRead(setPin) == LOW) && (pressedC == true)){
      BtnEnd = millis();
      BtnDauer = BtnEnd - BtnStart;
      if (BtnDauer > 50) { // Lange gedrückt
        plainText += currentCharacter;
        currentCharacter = "";
        morseCode = "";
        Ausgabe();
        textType++;
        
      } 
      pressedC = false;
      delay(20);
      }

}


void Ausgabe(){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(plainText);
    lcd.setCursor(0,1);
    lcd.print(morseCode); 
}

void Translate(){
  currentCharacter = "";
  
  String translateArray[37][2] = { //Uebersetzung der Morsezeichen in Buchstaben
    {"A", ".-"},
    {"B", "-..."},
    {"C", "-.-."},
    {"D", "-.."},
    {"E", "."},
    {"F", "..-."},
    {"G", "--."},
    {"H", "...."},
    {"I", ".."},
    {"J", ".---"},
    {"K", "-.-"},
    {"L", ".-.."},
    {"M", "--"},
    {"N", "-."},
    {"O", "---"},
    {"P", ".--."},
    {"Q", "--.-"},
    {"R", ".-."},
    {"S", "..."},
    {"T", "-"},
    {"U", "..-"},
    {"V", "...-"},
    {"W", ".--"},
    {"X", "-..-"},
    {"Y", "-.--"},
    {"Z", "--.."},
    {"1", ".----"},
    {"2", "..---"},
    {"3", "...--"},
    {"4", "...-"},
    {"5", "....."},
    {"6", "-...."},
    {"7", "--..."},
    {"8", "---.."},
    {"9", "----."},
    {"0", "-----"},
    {"_", ""}  //Leerzeichen bzw. Cursor
  };

  for (int i = 0; i < 37; i++){
    if (translateArray[i][1] == morseCode){
      currentCharacter = translateArray[i][0];
      break;
    }
  }
  
  if ((currentCharacter == "") && (morseCode != "")){ //Korrektur von Falscheingaben
    currentCharacter = "#";
  }
  
}
