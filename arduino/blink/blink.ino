// blink.ino - blink L LED to test development environment
// (c) BotBook.com - Karvinen, Karvinen, Valtokari
void setup() {	// <1>
  pinMode(13, OUTPUT);	// <2>
}

void loop() {	// <3>
  digitalWrite(13, HIGH);	// <4>
  delay(1000);	// ms	// <5>
  digitalWrite(13, LOW);
  delay(1000);
}

