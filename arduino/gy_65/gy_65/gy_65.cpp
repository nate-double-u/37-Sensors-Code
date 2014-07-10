// gy_65.cpp - library for altitude, pressure and temperature with GY-65 BMP085
// (c) BotBook.com - Karvinen, Karvinen, Valtokari
#include <Arduino.h>
#include <Wire.h>
#include "gy_65.h"
const char i2c_address = 0x77;
int OSS = 0; // Oversampling
const long atmosphereSeaLevel = 101325; // Pa

struct calibration_data	// <1>
{
  int16_t ac1;
  int16_t ac2;
  int16_t ac3;
  int16_t ac4;
  int16_t ac5;
  int16_t ac6;
  int16_t b1;
  int16_t b2;
  int16_t mb;
  int16_t mc;
  int16_t md;
};

calibration_data caldata;

long b5;

int16_t swap_int16_t(int16_t value)	// <2>
{
  int16_t left = value << 8;	
  int16_t right = value >> 8;	
  right = right & 0xFF;	
  return left | right ;
}

unsigned char read_i2c_unsigned_char(unsigned char address)	// <3>
{
  unsigned char data;
  Wire.beginTransmission(i2c_address);	
  Wire.write(address);	
  Wire.endTransmission(); 
  Wire.requestFrom(i2c_address,1);
  while(!Wire.available());
    return Wire.read();
}
void read_i2c(unsigned char point, uint8_t *buffer, int size)
{
  Wire.beginTransmission(i2c_address);
  Wire.write(point);
  Wire.endTransmission();

  Wire.requestFrom(i2c_address,size);

  int i = 0;

  while(Wire.available() && i < size) {	
    buffer[i] = Wire.read();
    i++;
  }
  
  if(i != size) {
    Serial.println("Error reading from i2c"); 
  }

}

int read_i2c_int(unsigned char address) {	
    int16_t data;
    read_i2c(address,(uint8_t *)&data,sizeof(int16_t));
    data = swap_int16_t(data); 
    return data;
}

void readCalibrationData()	// <4>
{
  Wire.begin();
  read_i2c(0xAA,(uint8_t *)&caldata,sizeof(calibration_data)); // <5>
  
  uint16_t *p = (uint16_t*)&caldata;	// <6>
  for(int i = 0; i < 11; i++) {	// <7>
    p[i] = swap_int16_t(p[i]);
  }  
}

float readTemperature() {	// <8>
  // Read raw temperature
  Wire.beginTransmission(i2c_address);
  Wire.write(0xF4); // Register
  Wire.write(0x2E); // Value
  Wire.endTransmission(); 
  delay(5); // <9>
  unsigned int rawTemp = read_i2c_int(0xF6);
  
  // Calculate true temperature
  long x1,x2;
  float t;
  x1 = (((long)rawTemp - (long)caldata.ac6) * (long)caldata.ac5) / pow(2,15);
  long mc = caldata.mc;
  int md = caldata.md;
  x2 = (mc * pow(2,11)) / (x1 + md); 
  b5 = x1 + x2;
  t = (b5 + 8) / pow(2,4); 
  t = t / 10;
  return t;	// Celsius
}

long getRealPressure(unsigned long up){	// <10>
  long x1, x2, x3, b3, b6, p;
  unsigned long b4, b7;
  int b1 = caldata.b1;
  int b2 = caldata.b2;
  long ac1 = caldata.ac1;
  int ac2 = caldata.ac2;
  int ac3 = caldata.ac3;
  unsigned int ac4 = caldata.ac4;  

  b6 = b5 - 4000;
  x1 = (b2 * (b6 * b6) / pow(2,12)) / pow(2,11);
  x2 = (ac2 * b6) / pow(2,11);
  x3 = x1 + x2;

  b3 = (((ac1*4 + x3) << OSS) + 2) / 4;
  x1 = (ac3 * b6) / pow(2,13);
  x2 = (b1 * ((b6 * b6) / pow(2,12)) ) / pow(2,16);
  x3 = ((x1 + x2) + 2) / 4;
  b4 = (ac4 * (unsigned long)(x3 + 32768) ) / pow(2,15);

  b7 = ((unsigned long)up - b3) * (50000 >> OSS);
  if (b7 < 0x80000000)  p = ( b7 * 2 ) / b4;
  else p = (b7 / b4) * 2;

  x1 = (p / pow(2,8)) * (p / pow(2,8));
  x1 = (x1 * 3038) / pow(2,16);
  x2 = (-7357 * p) / pow(2,16);
  p += (x1 + x2 + 3791) / pow(2,4);

  long temp = p;
  return temp;
}

float readPressure() {	// <11>
  // Read uncompensated pressure
  Wire.beginTransmission(i2c_address);
  Wire.write(0xF4); // Register
  Wire.write(0x34 + (OSS << 6)); // Value with oversampling setting.
  Wire.endTransmission();  
  
  delay(2 + (3 << OSS));
  
  unsigned char msb,lsb,xlsb;
  unsigned long rawPressure = 0;
  msb = read_i2c_unsigned_char(0xF6);
  lsb = read_i2c_unsigned_char(0xF7);
  xlsb = read_i2c_unsigned_char(0xF8);
  
  rawPressure = (((unsigned long) msb << 16) | 
        ((unsigned long) lsb << 8) | 
        (unsigned long) xlsb) >> (8-OSS);
  
  return getRealPressure(rawPressure);  
}

float calculateAltitude(float pressure) {	// <12>
  float pressurePart = pressure / atmosphereSeaLevel;
  float power = 1 / 5.255;
  float result = 1 - pow(pressurePart, power);
  float altitude = 44330*result;
  return altitude; // m
}



