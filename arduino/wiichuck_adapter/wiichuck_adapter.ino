// wiichuck_adapter.ino - print joystick, accelerometer and button data to serial
// (c) BotBook.com - Karvinen, Karvinen, Valtokari


#include <Wire.h>

const char i2c_address = 0x52;

unsigned long lastGet=0; // ms
int jx = 0, jy = 0, accX = 0, accY = 0, accZ = 0, buttonZ = 0, buttonC = 0;	// <1>

void setup() {
  Serial.begin(115200);
  Wire.begin();
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  digitalWrite(A2, LOW);	// <2>
  digitalWrite(A3, HIGH);	// <3>
  delay(100);
  initNunchuck();	// <4>
  
}

void loop() {
  if(millis() - lastGet > 100) {	// <5>
    get_data();	// <6>
    lastGet = millis();	// <7>
  }
  Serial.print("Button Z: ");
  Serial.print(buttonZ);	// <8>
  Serial.print(" Button C: ");
  Serial.print(buttonC);
  Serial.print(" Joystick: (x,y) (");
  Serial.print(jx);	// <9>
  Serial.print(",");
  Serial.print(jy); 
  Serial.print(") Acceleration (x,y,z) (");  
  Serial.print(accX);	// <10>
  Serial.print(",");
  Serial.print(accY); 
  Serial.print(",");
  Serial.print(accZ);   
  Serial.println(")");

  delay(10); // ms
}

void get_data() {
  int buffer[6]; // <11>
  Wire.requestFrom(i2c_address, 6);	// <12>
  int i = 0;	// <13>
  while(Wire.available()) {	// <14>
    buffer[i] = Wire.read();	// <15>
    buffer[i] ^= 0x17;	// <16>
    buffer[i] += 0x17;	// <17>
    i++;	
  }
  if(i != 6) {	// <18>
    Serial.println("Error reading from i2c"); 
  }  
  write_i2c_zero();	// <19>

  buttonZ = buffer[5] & 0x01;	// <20>
  buttonC = (buffer[5] >> 1) & 0x01;	// <21>
  jx = buffer[0];	// <22>
  jy = buffer[1];  
  accX = buffer[2];
  accY = buffer[3];
  accZ = buffer[4];  
 
}

void write_i2c_zero() {
  Wire.beginTransmission(i2c_address);	
  Wire.write(0x00);	
  Wire.endTransmission();
}

void initNunchuck() 	
{
  Wire.beginTransmission(i2c_address);	
  Wire.write(0x40);
  Wire.write(0x00);	
  Wire.endTransmission();
}


