// ky_026_flame.ino - report level IR light from flame to serial
// (c) BotBook.com - Karvinen, Karvinen, Valtokari

const int analogPin = A0;
const int digitalPin = 8;
const int ledPin = 13;

void setup() { 
  Serial.begin(115200);
  pinMode(digitalPin,INPUT);  
  pinMode(ledPin,OUTPUT);
}

void loop() 
{
  int threshold = -1; // HIGH or LOW
  int value = -1; // 0..1023
  value = analogRead(analogPin);	// <1>
  threshold = digitalRead(digitalPin);	// <2>
  Serial.print("Raw: ");
  Serial.print(value);
  Serial.print(" Over threshold: ");
  Serial.println(threshold);
  delay(10);
  if(threshold==HIGH) {	// <3>
    digitalWrite(ledPin, HIGH);  
  } else {
    digitalWrite(ledPin, LOW);  
  }
}

