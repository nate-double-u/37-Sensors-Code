// mpu_6050.ino - print acceleration (m/s**2) and angular velocity (gyro, deg/s)
// (c) BotBook.com - Karvinen, Karvinen, Valtokari

#include <Wire.h>	// <1>

const char i2c_address =  0x68; 	// <2>

const unsigned char sleep_mgmt = 0x6B;	// <3>
const unsigned char accel_x_out = 0x3B;

struct data_pdu	// <4>
{
  int16_t x_accel;	// <5>
  int16_t y_accel;
  int16_t z_accel;
  int16_t temperature;	// <6>
  int16_t x_gyro;	// <7>
  int16_t y_gyro;
  int16_t z_gyro;
};


void setup() {
  Serial.begin(115200); 
  Wire.begin();	// <8>
  write_i2c(sleep_mgmt,0x00);   // <9>
}

int16_t swap_int16_t(int16_t value)	// <10>
{
  int16_t left = value << 8;	// <11>
  int16_t right = value >> 8;	// <12>
  right = right & 0xFF;	// <13>
  return left | right;	// <14>
}

void loop() {
  data_pdu pdu;	// <15>
  read_i2c(accel_x_out, (uint8_t *)&pdu, sizeof(data_pdu));	// <16>

  pdu.x_accel = swap_int16_t(pdu.x_accel);	// <17>
  pdu.y_accel = swap_int16_t(pdu.y_accel);
  pdu.z_accel = swap_int16_t(pdu.z_accel);
  pdu.temperature = swap_int16_t(pdu.temperature);	// <18>
  pdu.x_gyro = swap_int16_t(pdu.x_gyro);
  pdu.y_gyro = swap_int16_t(pdu.y_gyro);
  pdu.z_gyro = swap_int16_t(pdu.z_gyro);

  float acc_x = pdu.x_accel / 16384.0f;	// <19>
  float acc_y = pdu.y_accel / 16384.0f;
  float acc_z = pdu.z_accel / 16384.0f;
  Serial.print("Accelerometer: x,y,z (");
  Serial.print(acc_x,3); Serial.print("g, ");	// <20>
  Serial.print(acc_y,3); Serial.print("g, ");
  Serial.print(acc_z,3); Serial.println("g)");
  
  int zero_point = -512 - (340 * 35);	// <21>
  double temperature = (pdu.temperature - zero_point) / 340.0;	// <22>
  Serial.print("Temperature (C): ");
  Serial.println(temperature,2);
  
  Serial.print("Gyro: x,y,z (");
  Serial.print(pdu.x_gyro / 131.0f); Serial.print(" deg/s, ");	// <23>
  Serial.print(pdu.y_gyro / 131.0f); Serial.print(" deg/s, ");
  Serial.print(pdu.z_gyro / 131.0f); Serial.println(" deg/s)"); 
  delay(1000);
}

void read_i2c(unsigned char reg, uint8_t *buffer, int size)	// <24>
{
  Wire.beginTransmission(i2c_address);	// <25>
  Wire.write(reg);	// <26>
  Wire.endTransmission(false);	// <27>
  Wire.requestFrom(i2c_address,size,true);	// <28>

  int i = 0;	// <29>
  while(Wire.available() && i < size) {	// <30>
    buffer[i] = Wire.read();	// <31>
    i++;
  }
  if(i != size) {	// <32>
    Serial.println("Error reading from i2c"); 
  }

}

void write_i2c(unsigned char reg, const uint8_t data) 	// <33>
{
  Wire.beginTransmission(i2c_address);	// <34>
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission(true);
}




