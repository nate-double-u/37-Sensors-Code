// adjustable_infrared_sensor_switch.ino - print detection to serial and light LED.
// (c) BotBook.com - Karvinen, Karvinen, Valtokari

const int sensorPin = 8;
const int ledPin = 13;

//Sensor value
int switchState = 0;

void setup() {
  Serial.begin(115200);	// <1>
  pinMode(sensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  switchState = digitalRead(sensorPin);	// <2>
  Serial.println(switchState);	// <3> 
  if(switchState == 0) {
    digitalWrite(ledPin, HIGH);
    Serial.println("Object detected!");	// <4>
  } else {
    digitalWrite(ledPin, LOW);
  }  
  delay(10); // ms	// <5>
}


