// line_sensor_black_or_white.ino - line follow sensor. Signal low when over black line.
// (c) BotBook.com - Karvinen, Karvinen, Valtokari


const int sensorPin = 2;
const int ledPin = 13;
int lineFound = -1;

void setup() {
  Serial.begin(115200);  
  pinMode(sensorPin, INPUT);
  // No need for pull-up as sensor has this already.
  pinMode(ledPin, OUTPUT);
}

void loop() {
  lineFound = digitalRead(sensorPin);
  if(lineFound == 1) {
    Serial.println("BLACK");
    digitalWrite(ledPin, HIGH); 
  } else {
    Serial.println("WHITE"); 
    digitalWrite(ledPin, LOW);
  }
  delay(50);
}



