// soil_humidity_sensor.ino - read soil humidity by measuring its resistance.
// (c) BotBook.com - Karvinen, Karvinen, Valtokari


const int sensorPin = A0;
int soilHumidity = -1;

void setup() {
  Serial.begin(115200);  
}

void loop() {
  soilHumidity = analogRead(sensorPin);	// <1>
  Serial.println(soilHumidity);
  delay(100);
}



