// gy_65.ino - print altitude, pressure and temperature with GY-65 BMP085
// (c) BotBook.com - Karvinen, Karvinen, Valtokari

#include <Wire.h>
#include <gy_65.h>	// <1>

void setup() {
  Serial.begin(115200);
  readCalibrationData(); // <2>
}

void loop() {
  float temp = readTemperature();
  float pressure = readPressure();	// <3>
  float altitude = calculateAltitude(pressure);	// <4>
  
  Serial.print("Altitude: ");
  Serial.print(altitude,2);
  Serial.println(" m");
  Serial.print("Pressure: ");
  Serial.print(pressure,2);
  Serial.println(" Pa");
  Serial.print("Temperature: ");
  Serial.print(temp,2);
  Serial.println("C");  
  delay(1000);  
}

