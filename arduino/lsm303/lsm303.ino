// lsm303.ino - normal use and calibration of LSM303DLH compass-accelerometer
// (c) BotBook.com - Karvinen, Karvinen, Valtokari


#include <Wire.h>

const char accelerometer_address = 0x30 >> 1;	// <1>
const char magnetometer_address = 0x3C >> 1;

const int runningMode = 0;	// <2>

float magMax_x = 0.1; float magMax_y = 0.1; float magMax_z = 0.1;	// <3>
float magMin_x = -0.1; float magMin_y = -0.1; float magMin_z = -0.1;

float acc_x = 0; float acc_y = 0; float acc_z = 0; // <4>
float mag_x = 0; float mag_y = 0; float mag_z = 0;

int heading = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();	// <5>
  Serial.println("Initialize compass");
  initializelsm();	// <6>
  delay(100);
  Serial.println("Start reading heading");
}

void loop() {
  updateHeading();	// <7>
  if(runningMode == 0) {	// <8>
    magMax_x = max(magMax_x, mag_x);    
    magMax_y = max(magMax_y, mag_y);    
    magMax_z = max(magMax_z, mag_z);      
    magMin_x = min(magMin_x, mag_x);
    magMin_y = min(magMin_y, mag_y);
    magMin_z = min(magMin_z, mag_z);    
    Serial.print("Max x y z: ");
    Serial.print(magMax_x); Serial.print(" ");  
    Serial.print(magMax_y); Serial.print(" ");
    Serial.print(magMax_z); Serial.print(" ");    
    Serial.print("Min x y z: ");  
    Serial.print(magMin_x); Serial.print(" ");  
    Serial.print(magMin_y); Serial.print(" ");
    Serial.print(magMin_z); Serial.println(" ");      
  } else {
    calculateHeading();
    Serial.println(heading);	// <9>
  }
  delay(100); // ms
}


void initializelsm() {
    write_i2c(accelerometer_address, 0x20, 0x27);	// <10>
    write_i2c(magnetometer_address, 0x02, 0x00);	// <11>
}

void updateHeading() {
  updateAccelerometer();
  updateMagnetometer();
}

void updateAccelerometer() {	// <12>
  Wire.beginTransmission(accelerometer_address);
  Wire.write(0x28 | 0x80);	// <13>
  Wire.endTransmission(false); 
  Wire.requestFrom(accelerometer_address, 6, true);	// <14>
  int i = 0;
  while(Wire.available() < 6) {
    i++;
    if(i > 1000) {
      Serial.println("Error reading from accelerometer i2c");
      return;
    }  
  }
  uint8_t  axel_x_l = Wire.read();	// <15>
  uint8_t  axel_x_h = Wire.read();
  uint8_t  axel_y_l = Wire.read();
  uint8_t  axel_y_h = Wire.read();
  uint8_t  axel_z_l = Wire.read();
  uint8_t  axel_z_h = Wire.read();  
  
  acc_x = (axel_x_l | axel_x_h << 8) >> 4; // <16>
  acc_y = (axel_y_l | axel_y_h << 8) >> 4; 
  acc_z = (axel_z_l | axel_z_h << 8) >> 4; 
  
}

void updateMagnetometer() { // <17>
  Wire.beginTransmission(magnetometer_address);
  Wire.write(0x03);
  Wire.endTransmission(false); 
  Wire.requestFrom(magnetometer_address, 6, true);
  int i = 0;
  while(Wire.available() < 6) {
    i++;
    if(i > 1000) {
      Serial.println("Error reading from magnetometer i2c");
      return;
    }  
  }
  uint8_t  axel_x_h = Wire.read();
  uint8_t  axel_x_l = Wire.read();
  uint8_t  axel_y_h = Wire.read();
  uint8_t  axel_y_l = Wire.read();
  uint8_t  axel_z_h = Wire.read();
  uint8_t  axel_z_l = Wire.read();  

  mag_x = (int16_t)(axel_x_l | axel_x_h << 8); 
  mag_y = (int16_t)(axel_y_l | axel_y_h << 8); 
  mag_z = (int16_t)(axel_z_l | axel_z_h << 8);   
}
//Heading to north
void calculateHeading() {
  // Up unit vector
  float dot = acc_x*acc_x + acc_y*acc_y + acc_z*acc_z;	// <18>
  float magnitude = sqrt(dot);	// <19>
  float nacc_x = acc_x / magnitude;	// <20>
  float nacc_y = acc_y / magnitude;
  float nacc_z = acc_z / magnitude;  
  
  // Apply calibration
  mag_x = (mag_x - magMin_x) / (magMax_x - magMin_x) * 2 - 1.0;	// <21>
  mag_y = (mag_y - magMin_y) / (magMax_y - magMin_y) * 2 - 1.0;
  mag_z = (mag_z - magMin_z) / (magMax_z - magMin_z) * 2 - 1.0;  
  
  // East
  float ex = mag_y*nacc_z - mag_z*nacc_y;	// <22>
  float ey = mag_z*nacc_x - mag_x*nacc_z;
  float ez = mag_x*nacc_y - mag_y*nacc_x;  
  dot = ex*ex + ey*ey + ez*ez; // <23>
  magnitude = sqrt(dot);
  ex /= magnitude;
  ey /= magnitude;
  ez /= magnitude;	// <24>

  

  // Project
  float ny = nacc_z*ex - nacc_x*ez;	// <25>
  
  float dotE = -1 * ey;	// <26>
  float dotN = -1 * ny;  
  
  // Angle
  heading = atan2(dotE, dotN) * 180 / M_PI;	// <27>
  
  heading = round(heading); 
  if (heading < 0) heading +=360; // <28>
}

void write_i2c(char address, unsigned char reg, const uint8_t data) 	
{
  Wire.beginTransmission(address);	
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}


