// pot.ino - control LED blinking speed with potentiometer
// (c) BotBook.com - Karvinen, Karvinen, Valtokari


int potPin=A0; // <1>
int ledPin=13;
int x=0; // 0..1023

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(potPin, INPUT);
}

void loop() {
  x=analogRead(potPin); // <2>
  digitalWrite(ledPin, HIGH);
  delay(x/10); // ms	// <3>
  digitalWrite(ledPin, LOW);
  delay(x/10);
}

