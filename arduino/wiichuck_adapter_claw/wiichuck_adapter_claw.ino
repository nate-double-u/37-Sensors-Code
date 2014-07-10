// wiichuck_adapter_claw.ino - control robot hand with Nunchuck
// (c) BotBook.com - Karvinen, Karvinen, Valtokari

#include <Wire.h>

const int clawPin = 8;
const int armPin = 9;
int armPos=0, clawPos=0;
float wiiP = 0.0;	// <1>
float wiiPAvg = 0.0;	// <2>
int lastarmPos = 350;

const char i2c_address = 0x52;
int jx = 0, jy = 0, accX = 0, accY = 0, accZ = 0, buttonZ = 0, buttonC = 0;

void setup() {
  Serial.begin(115200);

  // Nunchuck
  Wire.begin();
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  digitalWrite(A2, LOW);
  digitalWrite(A3, HIGH);
  delay(100);
  initNunchuck();
  
  // Servos
  pinMode(clawPin, OUTPUT);
  pinMode(armPin, OUTPUT);
}

void loop() {
  get_data();	// <3>

  wiiP = (accZ-70.0)/(178.0-70.0);	// <4>
  if (accY>120 && accZ>100) wiiP=1;
  if (accY>120 && accZ<100) wiiP=0;
  if (wiiP>1) wiiP=1;
  if (wiiP<0) wiiP=0;
  wiiPAvg = runningAvg(wiiP, wiiPAvg);	// <5>
  armPos = map(wiiPAvg*10*1000, 0, 10*1000, 2200, 350);
  
  clawPos = map(jy, 30, 220, 1600, 2250);	// <6>
  
  pulseServo(armPin, armPos);	// <7>
  pulseServo(clawPin, clawPos);

  printDebug();
}

float runningAvg(float current, float old) {
  float newWeight=0.3;
  return newWeight*current + (1-newWeight)*old;	// <8>
}

// servo

void pulseServo(int servoPin, int pulseLenUs)	// <9>
{
  digitalWrite(servoPin, HIGH);
  delayMicroseconds(pulseLenUs);
  digitalWrite(servoPin, LOW);
  delay(15);
}

// i2c

void get_data() {
  int buffer[6];
  Wire.requestFrom(i2c_address,6);
  int i = 0;	
  while(Wire.available()) {	
    buffer[i] = Wire.read();
    buffer[i] ^= 0x17;
    buffer[i] += 0x17;
    i++;	
  }
  if(i != 6) {	
    Serial.println("Error reading from i2c"); 
  }  
  write_i2c_zero();

  buttonZ = buffer[5] & 0x01;
  buttonC = (buffer[5] >> 1) & 0x01;
  jx = buffer[0];
  jy = buffer[1];  
  accX = buffer[2];
  accY = buffer[3];
  accZ = buffer[4];  
 
}

void write_i2c_zero() {
  Wire.beginTransmission(i2c_address);	
  Wire.write((byte)0x00);	
  Wire.endTransmission();	  
}

void initNunchuck() 	
{
  Wire.beginTransmission(i2c_address);	
  Wire.write((byte)0x40);
  Wire.write((byte)0x00);	
  Wire.endTransmission();	
}

// debug

void printDebug()
{
  Serial.print("accZ:");
  Serial.print(accZ);
  Serial.print("	wiiP:");
  Serial.print(wiiP);
  Serial.print("	wiiPAvg:");
  Serial.print(wiiPAvg);
  Serial.print("	jy:");
  Serial.print(jy);
  Serial.print("	clawPos:");
  Serial.println(clawPos);
}
