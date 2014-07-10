// weather_station.ino - print weather data to epaper
// (c) BotBook.com - Karvinen, Karvinen, Valtokari

#include <inttypes.h>
#include <ctype.h>

#include <SPI.h>
#include <Wire.h>
#include <EPD.h>	// <1>
#include <gy_65.h>	// <2>
#include <avr/sleep.h>
#include <avr/power.h>

#include "rain.h"	// <3>
#include "sun.h"
#include "suncloud.h"
#include "fonts.h"

uint8_t imageBuffer[5808]; // 264 * 176 / 8

const int pinPanelOn = 2;
const int pinBorder = 3;
const int pinDischarge = 4;
const int pinPWM = 5;
const int pinReset = 6;
const int pinBusy = 7;
const int pinEPDcs = 8;

EPD_Class EPD(EPD_2_7, 
              pinPanelOn,
              pinBorder,
              pinDischarge,
              pinPWM,
              pinReset,
              pinBusy,
              pinEPDcs,
              SPI);

float weatherDiff;
float temperature;

const int sleepMaxCount = 10;	// min
volatile int arduinoSleepingCount = sleepMaxCount;

void setup() {
  Serial.begin(115200);
  pinMode(pinPanelOn, OUTPUT);
  pinMode(pinBorder, OUTPUT);
  pinMode(pinDischarge, INPUT);
  pinMode(pinPWM, OUTPUT);
  pinMode(pinReset, OUTPUT);
  pinMode(pinBusy, OUTPUT);
  pinMode(pinEPDcs, OUTPUT);


  digitalWrite(pinPWM, LOW);
  digitalWrite(pinReset, LOW);
  digitalWrite(pinPanelOn, LOW);
  digitalWrite(pinDischarge, LOW);
  digitalWrite(pinBorder, LOW);
  digitalWrite(pinEPDcs, LOW);

  SPI.begin();	// <4>
  SPI.setBitOrder(MSBFIRST);	// <5>
  SPI.setDataMode(SPI_MODE0);	// <6>
  SPI.setClockDivider(SPI_CLOCK_DIV4);  // <7>
  
  WDTCSR |= (1<<WDCE) | (1<<WDE);	// <8>
  WDTCSR = 1<<WDP0 | 1<<WDP3;	// <9>
  WDTCSR |= _BV(WDIE);	// <10>
  MCUSR &= ~( 1 << WDRF);	// <11>

  for(int i = 0; i < 5808; i++)	// <12>
    imageBuffer[i] = 0;
  
  readCalibrationData();	// <13>
 
}

char characterMap[14] = {'+', '-', 'C', 'd', 
                         '0', '1', '2', '3',
                         '4', '5', '6', '7',
                         '8', '9'};	// <14>

void drawCharacter(int16_t x, int16_t y, const uint8_t *bitmap, char character) {
  int charIndex = -1;
  for(int i = 0; i < 14; i++) {	// <15>
    if(character == characterMap[i]) {
      charIndex = i;
      break;
    }  
  }
  if(charIndex == -1) return;
  drawBitmap(x,y,bitmap,charIndex*25,0,25,27,350);	// <16>
}  

void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t x2, 
    int16_t y2, int16_t w, int16_t h, int16_t source_width) {	// <17>

  int16_t i, j, byteWidth = source_width / 8;

  for(j=y2; j<y2+h; j++) {	// <18>
    for(i=x2; i<x2+w; i++ ) {
      byte b= pgm_read_byte(bitmap+j * byteWidth + i / 8);
      if(b & (128 >> (i & 7))) {
	     drawPixel(x+i-x2, y+j-y2, true);
      }
    }
  }
}

void drawPixel(int x, int y, bool black) { // <19>
  int bit = x & 0x07;
  int byte = x / 8 + y * (264 / 8);
  int mask = 0x01 << bit;
  if(black == true) {
    imageBuffer[byte] |= mask;  
  } else {
    imageBuffer[byte] &= ~mask;    
  }
}

void drawBufferToScreen() {	// <20>
  for (uint8_t line = 0; line < 176 ; ++line) {
	EPD.line(line, &imageBuffer[line * (264 / 8)], 0, false, EPD_inverse);
  }  
  for (uint8_t line = 0; line < 176 ; ++line) {
	EPD.line(line, &imageBuffer[line * (264 / 8)], 0, false, EPD_normal);
  }  
}

void drawScreen() {	// <21>
  EPD.begin();
  EPD.setFactor(temperature);
  EPD.clear();
  if(weatherDiff > 250) {	// Pa
    // Sunny
    drawBitmap(140,30,sun,0,0,117,106,117);	// <22>
  } else if ((weatherDiff <= 250) || (weatherDiff >= -250)) {
    // Partly cloudy
    drawBitmap(140,30,suncloud,0,0,117,106,117);
  } else if (weatherDiff < -250) {
    // Rain
    drawBitmap(140,30,rain,0,0,117,106,117);
  }
  //Draw temperature
  String temp = String((int)temperature);

  int pos = 10;
  drawCharacter(pos,70,font,'+');	// <23>
  pos += 25;
  drawCharacter(pos,70,font,temp.charAt(0));
  pos += 25;  
  if(abs(temperature) >= 10) {
    drawCharacter(pos,70,font,temp.charAt(1)); 
    pos += 25;     
  } 
  drawCharacter(pos,70,font,'d');
  pos += 25;  
  drawCharacter(pos,70,font,'C');  
  
  
  drawBufferToScreen();	// <24>
  
  EPD.end();

  for(int i = 0; i < 5808; i++)	// <25>
    imageBuffer[i] = 0;  
}

void loop() {
  Serial.println(temperature);	// <26>
  if(arduinoSleepingCount >= sleepMaxCount) {	// <27>
    readWeatherData();	// <28>
    drawScreen();
    arduinoSleepingCount = 0; // <29>
    arduinoSleep(); // <30>
  } else {
    arduinoSleep();
  }
}

const float currentAltitude = 40.00; // Installation altitude in meters
const long atmosphereSeaLevel = 101325; // Pa
const float expectedPressure = atmosphereSeaLevel * pow((1-currentAltitude / 44330), 5.255);

void readWeatherData(){
  temperature = readTemperature();
  float pressure = readPressure();
  weatherDiff = pressure - expectedPressure;
}

ISR(WDT_vect)	// <31>
{
  arduinoSleepingCount++;
}

void arduinoSleep() { // <32>
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);  
  sleep_enable();  
  sleep_mode(); // <33>
  sleep_disable();	// <34>
  power_all_enable();  
} // <35>
