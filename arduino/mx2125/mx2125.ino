// mx2125.ino - measure acceleration on two axes using MX2125 and print to serial
// (c) BotBook.com - Karvinen, Karvinen, Valtokari

const int xPin = 8;
const int yPin = 9;


void setup() {
  Serial.begin(115200);  
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
}

void loop() {
  int x = pulseIn(xPin, HIGH);	// <1>
  int y = pulseIn(yPin, HIGH);
  int x_mg = ((x / 10) - 500) * 8;	// <2>
  int y_mg = ((y / 10) - 500) * 8;  
  Serial.print("Axels x: ");
  Serial.print(x_mg);
  Serial.print(" y: ");
  Serial.println(y_mg);
  delay(10);
}



