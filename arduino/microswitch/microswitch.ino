// microswitch.ino - print to serial if microswitch is down or up
// (c) BotBook.com - Karvinen, Karvinen, Valtokari


const int switchPin = 2;
int switchState = -1;	// <1>

void setup() {
  Serial.begin(115200);
  pinMode(switchPin, INPUT);
  digitalWrite(switchPin, HIGH); // internal pull-up // <2>
}

void loop() {
  switchState = digitalRead(switchPin);
  Serial.println(switchState);	// <3>
  delay(10);
}



