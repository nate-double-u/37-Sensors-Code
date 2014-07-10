// ldr_light_sensor.ino - report high level of light with built-in LED
// (c) BotBook.com - Karvinen, Karvinen, Valtokari

const int lightPin = A0;
const int ledPin = 13;
int lightLevel = -1;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  lightLevel = analogRead(lightPin);	// <1>
  Serial.println(lightLevel);
  if(lightLevel < 400) {	// <2>
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
  delay(10);
}

