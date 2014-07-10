# lsm303.py - normal use and calibration of LSM303DLH compass-accelerometer
# (c) BotBook.com - Karvinen, Karvinen, Valtokari
import time
import smbus # sudo apt-get -y install python-smbus
import struct
import math

accelerometer_address = 0x30 >> 1
magnetometer_address = 0x3C >> 1

calibrationMode = True	# <1>

magMax = [ 0.1, 0.1, 0.1 ]	# <2>
magMin = [ 0.1, 0.1, 0.1 ]

acc = [ 0.0, 0.0, 0.0 ]	# <3>
mag = [ 0.0, 0.0, 0.0 ]

heading = 0

def initlsm():
  global bus	
  bus = smbus.SMBus(1)
  bus.write_byte_data(accelerometer_address, 0x20, 0x27)	# <4>
  bus.write_byte_data(magnetometer_address, 0x02, 0x00)  # <5>

def updateAccelerometer():
  global acc	# <6>
  bus.write_byte(accelerometer_address, 0x28 | 0x80)	# <7>
  rawData = ""
  for i in range(6):
    rawData += chr(bus.read_byte_data(accelerometer_address, 0x28+i)) # <8>

  data = struct.unpack('<hhh',rawData) # <9>
  acc[0] = data[0] >> 4	# <10>
  acc[1] = data[1] >> 4
  acc[2] = data[2] >> 4

def updateMagnetometer():	# <11>
  global mag
  bus.write_byte(magnetometer_address, 0x03)
  rawData = ""
  for i in range(6):
    rawData += chr(bus.read_byte_data(magnetometer_address, 0x03+i))
  
  data = struct.unpack('>hhh',rawData)
  mag[0] = data[0]
  mag[1] = data[1]
  mag[2] = data[2]
  

def calculateHeading():
  global heading, acc, mag
  #normalize
  normalize(acc)	# <12>
  
  #use calibration data
  mag[0] = (mag[0] - magMin[0]) / (magMax[0] - magMin[0]) * 2.0 - 1.0	# <13>
  mag[1] = (mag[1] - magMin[1]) / (magMax[1] - magMin[1]) * 2.0 - 1.0
  mag[2] = (mag[2] - magMin[2]) / (magMax[2] - magMin[2]) * 2.0 - 1.0
  
  e = cross(mag, acc)	# <14>
  normalize(e)	# <15>
  
  n = cross(acc,e)	# <16>
  
  dotE = dot(e,[0.0, -1.0, 0.0])	# <17>
  dotN = dot(n,[0.0, -1.0, 0.0])
  
  heading = round(math.atan2(dotE, dotN) * 180.0 / math.pi)	# <18>
  if heading < 0:	# <19>
    heading += 360	# <20>

def normalize(v):	# <21>
  magnitude = math.sqrt(dot(v,v))
  v[0] /= magnitude
  v[1] /= magnitude
  v[2] /= magnitude

def dot(v1, v2):	# <22>
  return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2]

def cross(v1, v2):	# <23>
  vr = [0.0, 0.0, 0.0]
  vr[0] = v1[1] * v2[2] - v1[2] * v2[1]
  vr[1] = v1[2] * v2[0] - v1[0] * v2[2]
  vr[2] = v1[0] * v2[1] - v1[1] * v2[0]
  return vr

def main():
  initlsm()
  while True:	
    updateAccelerometer()	# <24>
    updateMagnetometer()
    
    if calibrationMode:	# <25>
      magMax[0] = max(magMax[0], mag[0])
      magMax[1] = max(magMax[1], mag[1])
      magMax[2] = max(magMax[2], mag[2])
      magMin[0] = min(magMin[0], mag[0])
      magMin[1] = min(magMin[1], mag[1])
      magMin[2] = min(magMin[2], mag[2])
      print("magMax = [ %.1f, %.1f, %.1f ]" % (magMax[0], magMax[1], magMax[2]))
      print("magMin = [ %.1f, %.1f, %.1f ]" % (magMin[0], magMin[1], magMin[2]))
    else:
      calculateHeading()	# <26>
      print(heading)
    time.sleep(0.5) 

if __name__ == "__main__":
  main()
