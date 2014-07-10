// microphone_can_you_hear_pin_drop.ino - print to serial when sound is heard
// (c) BotBook.com - Karvinen, Karvinen, Valtokari


const int audioPin = A0;

void setup() {
  Serial.begin(115200);  
}

void loop() {
  int soundWave = analogRead(audioPin);	// 1
  if (soundWave>600) {	// 2
    Serial.println("Sound!");
  }  
  delay(10);
}

