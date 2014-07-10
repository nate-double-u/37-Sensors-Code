// tilt_sensor.ino - detect tilting and print to serial
// (c) BotBook.com - Karvinen, Karvinen, Valtokari

const int tiltPin = 8;
int tilted = -1;

void setup() {
  Serial.begin(115200);  
  pinMode(tiltPin, INPUT);
  digitalWrite(tiltPin, HIGH);
}

void loop() {
  tilted = digitalRead(tiltPin);	// <1>
  if(tilted == 0) {
    Serial.println("Sensor is tilted");  
  } else {
    Serial.println("Sensor is not tilted");    
  }

  delay(100);
}



