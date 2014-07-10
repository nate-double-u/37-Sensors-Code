// hall_sensor.ino - print raw value and magnets pole
// (c) BotBook.com - Karvinen, Karvinen, Valtokari

const int hallPin = A0;
int rawMagneticStrength = -1;	// <1>
int zeroLevel = 527;	// <2>

void setup() {
  Serial.begin(115200);	
  pinMode(hallPin, INPUT);
}

void loop() {
  rawMagneticStrength = analogRead(hallPin);	// <3>
  Serial.print("Raw strength: ");	
  Serial.println(rawMagneticStrength);
  int zeroedStrength = rawMagneticStrength - zeroLevel;
  // If you know your Hall sensor's conversion from 
  // voltage to gauss then you can do it here
  // zeroedStrength * conversion
  Serial.print("Zeroed strength: ");
  Serial.println(zeroedStrength);
  if(zeroedStrength > 0) {
    Serial.println("South pole"); 
  } else if(zeroedStrength < 0) {
    Serial.println("North pole"); 
  }   
  delay(600); // ms
}

