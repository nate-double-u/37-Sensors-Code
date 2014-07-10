# mpu_6050.py - print acceleration (m/s**2) and angular velocity (gyro, deg/s)
# (c) BotBook.com - Karvinen, Karvinen, Valtokari
import time
import smbus # sudo apt-get -y install python-smbus  # <1>
import struct

i2c_address =  0x68	# <2>
sleep_mgmt = 0x6B	# <3>
accel_x_out = 0x3B	# <4>

bus = None	# <5>
acc_x = 0
acc_y = 0
acc_z = 0
temp = 0
gyro_x = 0
gyro_y = 0
gyro_z = 0


def initmpu():
  global bus	# <6>
  bus = smbus.SMBus(1)	# <7>
  bus.write_byte_data(i2c_address, sleep_mgmt, 0x00)	# <8>

def get_data():
  global acc_x,acc_y,acc_z,temp,gyro_x,gyro_y,gyro_z
  bus.write_byte(i2c_address, accel_x_out)	# <9>
  rawData = ""
  for i in range(14):	# <10>
    rawData += chr(bus.read_byte_data(i2c_address,accel_x_out+i))	# <11>
  data = struct.unpack('>hhhhhhh', rawData)	# <12>

  acc_x = data[0] / 16384.0	# <13>
  acc_y = data[1] / 16384.0
  acc_z = data[2] / 16384.0
  zero_point = -512 - (340 * 35)	# <14>
  temp = (data[3] - zero_point) / 340.0	# <15>

  gyro_x = data[4] / 131.0	# <16>
  gyro_y = data[5] / 131.0
  gyro_z = data[6] / 131.0


def main():
  initmpu()
  while True:	# <17>
    get_data()	# <18>
    print("DATA:")
    print("Acc (%.3f,%.3f,%.3f) g, " % (acc_x, acc_y, acc_z))	# <19>
    print("temp %.1f C, " % temp)
    print("gyro (%.3f,%.3f,%.3f) deg/s" % (gyro_x, gyro_y, gyro_z))    
    time.sleep(0.5) # s	# <20>

if __name__ == "__main__":
  main()
