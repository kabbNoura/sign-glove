#include <Wire.h>
#include <LiquidCrystal_I2C.h>
 
LiquidCrystal_I2C lcd(0x27, 16, 2);
 
const int flexPins[5] = {A0, A1, A2, A3, A4};
const int buttonPin = 7;        // bouton reset sur pin 7
 
int v[5];
String currentMessage = "...";  // message affiche fige
bool captured = false;          // a-t-on deja capture un geste ?
 
void setup() {
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
 
  lcd.setCursor(0, 0);
  lcd.print("Sign Glove");
  delay(1500);
  lcd.clear();
}
 
bool inRange(int val, int minVal, int maxVal) {
  return (val >= minVal && val <= maxVal);
}
 
// Lit les sensors et retourne le message detecte
String detectGesture() {
  for (int i = 0; i < 5; i++) {
    v[i] = analogRead(flexPins[i]);
  }
 
  // >>> Affiche les 5 valeurs dans le Serial Monitor <<<
  Serial.print("A0="); Serial.print(v[0]);
  Serial.print("  A1="); Serial.print(v[1]);
  Serial.print("  A2="); Serial.print(v[2]);
  Serial.print("  A3="); Serial.print(v[3]);
  Serial.print("  A4="); Serial.print(v[4]);
  Serial.println();
 
  String message = "?";
 
  if (inRange(v[0],150,200) && inRange(v[1],0,39)   && inRange(v[2],0,87)   && inRange(v[3],0,110)   && inRange(v[4],0,69))   message = "Hi";
  else if (inRange(v[0],0,149)   && inRange(v[1],40,200) && inRange(v[2],0,87)   && inRange(v[3],0,110)   && inRange(v[4],0,69))   message = "How are you";
  else if (inRange(v[0],0,149)   && inRange(v[1],0,39)   && inRange(v[2],0,88)   && inRange(v[3],0,110)   && inRange(v[4],70,100)) message = "I need the WC";
  else if (inRange(v[0],150,200) && inRange(v[1],40,200) && inRange(v[2],0,88)   && inRange(v[3],0,110)   && inRange(v[4],0,69))   message = "Not good";
  else if (inRange(v[0],150,200) && inRange(v[1],40,200) && inRange(v[2],88,200) && inRange(v[3],0,110)   && inRange(v[4],0,69))   message = "I'm ok";
  else if (inRange(v[0],150,200) && inRange(v[1],40,200) && inRange(v[2],88,200) && inRange(v[3],111,200) && inRange(v[4],0,69))   message = "I am in pain";
  else if (inRange(v[0],150,200) && inRange(v[1],40,200) && inRange(v[2],88,200) && inRange(v[3],111,200) && inRange(v[4],70,100)) message = "By";
  else if (inRange(v[0],0,149)   && inRange(v[1],0,39)   && inRange(v[2],0,87)   && inRange(v[3],0,110)   && inRange(v[4],0,69))   message = "Help!";
  else if (inRange(v[0],0,149)   && inRange(v[1],40,200) && inRange(v[2],88,200) && inRange(v[3],111,200) && inRange(v[4],70,100)) message = "I need water";
  else if (inRange(v[0],0,149)   && inRange(v[1],0,39)   && inRange(v[2],88,200) && inRange(v[3],110,200) && inRange(v[4],70,100)) message = "Yes";
  else if (inRange(v[0],0,149)   && inRange(v[1],0,39)   && inRange(v[2],0,87)   && inRange(v[3],110,200) && inRange(v[4],70,100)) message = "No";
  else if (inRange(v[0],0,149)   && inRange(v[1],40,200) && inRange(v[2],88,200) && inRange(v[3],0,110)   && inRange(v[4],0,69))   message = "Good";
  else if (inRange(v[0],0,149)   && inRange(v[1],40,200) && inRange(v[2],88,200) && inRange(v[3],111,200) && inRange(v[4],0,69))   message = "Happy";
  else if (inRange(v[0],0,149)   && inRange(v[1],40,200) && inRange(v[2],0,87)   && inRange(v[3],0,110)   && inRange(v[4],70,100)) message = "Sad";
  else if (inRange(v[0],150,200) && inRange(v[1],0,39)   && inRange(v[2],0,87)   && inRange(v[3],0,110)   && inRange(v[4],70,100)) message = "Love you";
  else if (inRange(v[0],150,200) && inRange(v[1],40,200) && inRange(v[2],0,87)   && inRange(v[3],0,110)   && inRange(v[4],70,100)) message = "Hungry";
  else if (inRange(v[0],0,149)   && inRange(v[1],40,200) && inRange(v[2],88,200) && inRange(v[3],0,110)   && inRange(v[4],70,100)) message = "Sick";
 
  return message;
}
 
void afficher(String message) {
  lcd.setCursor(0, 0);
  lcd.print("Message:        ");
  lcd.setCursor(0, 1);
  lcd.print(message);
  for (int k = message.length(); k < 16; k++) lcd.print(" ");
}
 
void capturer() {
  // Compte a rebours 3 s
  for (int s = 3; s > 0; s--) {
    lcd.setCursor(0, 0);
    lcd.print("Preparez: ");
    lcd.print(s);
    lcd.print("s   ");
    delay(1000);
  }
 
  // Lecture du geste
  currentMessage = detectGesture();
 
  // Attendre 1 s
  lcd.setCursor(0, 0);
  lcd.print("Lecture...      ");
  delay(1000);
 
  // Afficher et figer
  afficher(currentMessage);
  captured = true;
}
 
void loop() {
  // Premiere capture automatique au demarrage
  if (!captured) {
    capturer();
  }
 
  // Si on appuie sur le bouton -> nouvelle capture
  if (digitalRead(buttonPin) == LOW) {
    delay(50);   // anti-rebond
    if (digitalRead(buttonPin) == LOW) {
      captured = false;   // relance une capture
    }
  }
}
