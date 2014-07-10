// gy_65.h - library for altitude, pressure and temperature with GY-65 BMP085
// (c) BotBook.com - Karvinen, Karvinen, Valtokari

void readCalibrationData();
float readTemperature();
float readPressure();
float calculateAltitude(float pressure);
