// hall_switch.ino - write to serial if magnet triggers the switch
// (c) BotBook.com - Karvinen, Karvinen, Valtokari

int switchPin=2;
void setup() {
  Serial.begin(115200);
  pinMode(switchPin, INPUT); 
  digitalWrite(switchPin, HIGH); 
}

void loop() {
  int switchState=digitalRead(switchPin);	// <1>
  if (switchState == LOW) {
    Serial.println("YES, magnet is near");
  } else {
     Serial.println("no"); 
  }
  delay(50);
}


