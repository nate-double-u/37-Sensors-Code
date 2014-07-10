// button.ino - light an LED by pressing a button
// (c) BotBook.com - Karvinen, Karvinen, Valtokari


int buttonPin=2;
int ledPin=13;
int buttonStatus=-1;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT); // <1>
  digitalWrite(buttonPin, HIGH); // internal pull-up // <2>
}

void loop() {
  buttonStatus=digitalRead(buttonPin); // <3>
  if (LOW==buttonStatus) { // <4>
    digitalWrite(ledPin, HIGH); // <5>
  } else {
    digitalWrite(ledPin, LOW);
  }
  delay(20);	// <6>
}


