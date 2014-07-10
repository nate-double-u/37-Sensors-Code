// line_sensor.ino - print to serial if we are on a line
// (c) BotBook.com - Karvinen, Karvinen, Valtokari

const int sensorPin = 2;
const int ledPin = 13;
int lineFound = -1;

void setup() {
  Serial.begin(115200);  
  pinMode(sensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  lineFound = digitalRead(sensorPin);	// <1>
  if(lineFound == HIGH) {
    Serial.println("Sensor is on the line");
    digitalWrite(ledPin, HIGH); 
  } else {
    Serial.println("Sensor is off the line"); 
    digitalWrite(ledPin, LOW);
  }
  delay(10);
}



