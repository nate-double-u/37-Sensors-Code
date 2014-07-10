// qt113.ino - qt113 touch sensor
// (c) BotBook.com - Karvinen, Karvinen, Valtokari

int sensorPin = 9;

void setup()
{
  pinMode(sensorPin, INPUT);
  Serial.begin(115200);
}

void loop()
{
  int touch = digitalRead(sensorPin);	// <1>
  if(touch == LOW) {	
    Serial.println("Touch detected");
  } else {
    Serial.println("No Touch detected");
  }
  delay(100);
}

