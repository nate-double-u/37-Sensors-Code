// vibration_sensor.ino - detect vibration using interrupt
// (c) BotBook.com - Karvinen, Karvinen, Valtokari

const int sensorPin = 0; //UNO,Mega pin 2, Leonardo pin 3
volatile int sensorState = -1;

void setup() {
  Serial.begin(115200); 
  attachInterrupt(sensorPin, sensorTriggered, RISING);	// <1>
}

void loop() {
  if(sensorState == 1) {	// <2>
    Serial.println("Vibrations detected");
    delay(1);	// ms
    sensorState = 0;
  }
  delay(10);
}

void sensorTriggered() {	// <3>
  sensorState = 1;  // <4>
}




