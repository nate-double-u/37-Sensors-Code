// mq_x_smoke_sensor.ino - print smoke level to serial 
// (c) BotBook.com - Karvinen, Karvinen, Valtokari

const int sensorPin = A0;
int smoke_level = -1;	// <1>

void setup() {
  Serial.begin(115200);	// bit/s
  pinMode(sensorPin, INPUT);
}

void loop() {
  smoke_level = analogRead(sensorPin);	// <2>
  Serial.println(smoke_level);
  if(smoke_level > 120) {	// <3>
    Serial.println("Smoke detected"); 
  } 
  delay(100); // ms
}

