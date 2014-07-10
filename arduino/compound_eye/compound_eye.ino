// compound_eye.ino - print distance and direction values to serial
// (c) BotBook.com - Karvinen, Karvinen, Valtokari

const int irEnablePin = 8;	// <1>
const int irUpPin = 0;
const int irDownPin = 2;
const int irLeftPin = 1;
const int irRightPin = 3;

int distance = 0;	// <2>
int irUpValue = 0;
int irDownValue = 0;
int irLeftValue = 0;
int irRightValue = 0;

void setup() {
  Serial.begin(115200);
  pinMode(irEnablePin, OUTPUT);  
}

void loop() {
  readSensor(); // <3>
  Serial.print("Values: "); // <4>
  Serial.print("irUpValue"); Serial.print(irUpValue); Serial.print(",");
  Serial.print("irDownValue"); Serial.print(irDownValue); Serial.print(",");  
  Serial.print("irLeftValue"); Serial.print(irLeftValue); Serial.print(",");
  Serial.print("irRightValue"); Serial.print(irRightValue); Serial.print(",");  
  Serial.print("distance"); Serial.println(distance);
  delay(100);
}

void readSensor() {
  digitalWrite(irEnablePin, HIGH);	// <5>
  delay(5); // ms	// <6>
  irUpValue = analogRead(irUpPin);
  irDownValue = analogRead(irDownPin);
  irLeftValue = analogRead(irLeftPin);
  irRightValue = analogRead(irRightPin); 

  int ambientLight = 0;	// <7>
  digitalWrite(irEnablePin, LOW);	// <8>
  delay(5);
  ambientLight = analogRead(irUpPin);	// <9>
  irUpValue = irUpValue - ambientLight;	// <10>
  
  ambientLight = analogRead(irDownPin);
  irDownValue = irDownValue - ambientLight;
  
  ambientLight = analogRead(irLeftPin);
  irLeftValue = irLeftValue - ambientLight;
  
  ambientLight = analogRead(irRightPin); 
  irRightValue = irRightValue - ambientLight; 

  distance = (irUpValue+irDownValue+irLeftValue+irRightValue) / 4;	// <11>
}


