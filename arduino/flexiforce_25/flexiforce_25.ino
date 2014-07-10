// flexiforce_25.ino - send flexiforce squeeze values to computer serial monitor
// (c) BotBook.com - Karvinen, Karvinen, Valtokari
   
int squeezePin=A0;	// <1>
int x=-1; // 0..1023

void setup() {
  pinMode(squeezePin, INPUT);
  Serial.begin(115200); // bit/s
}

void loop() {
  x=analogRead(squeezePin);  // <2>
  Serial.println(x);
  delay(500); // ms
}

