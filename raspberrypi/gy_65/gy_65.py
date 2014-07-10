# gy_65.py - print altitude,pressure and temperature to serial
# (c) BotBook.com - Karvinen, Karvinen, Valtokari

import smbus # sudo apt-get -y install python-smbus	# <1>
import time
import struct

bus = None  
address = 0x77
caldata = None

atmosphereSeaLevel = 101325.0
OSS = 0
b5 = 0

def readCalibrationData():	# <2>
  global bus, caldata
  bus = smbus.SMBus(1)
  rawData = ""

  for i in range(22): 
    rawData += chr(bus.read_byte_data(address, 0xAA+i))	# <3>

  caldata = struct.unpack('>hhhhhhhhhhh', rawData) # <4>

def readTemperature():
  global b5
  bus.write_byte_data(address, 0xF4, 0x2E)	# <5>
  time.sleep(0.005)	# <6>
  rawTemp = bus.read_byte_data(address, 0xF6) << 8	# <7>
  rawTemp = rawTemp | bus.read_byte_data(address, 0xF7)
  x1 = ((rawTemp - caldata[5]) * caldata[4]) / 2**15
  x2 = (caldata[9] * 2**11) / (x1 + caldata[10])
  b5 = x1 + x2
  temp = (b5 + 8) / 2**4
  temp = temp / 10.0
  return temp


def readPressure():	# <8>
  bus.write_byte_data(address, 0xF4, 0x34 + (OSS << 6))
  time.sleep(0.005)
  rawPressure = bus.read_byte_data(address, 0xF6) << 16
  rawPressure = rawPressure | bus.read_byte_data(address, 0xF7) << 8
  rawPressure = rawPressure | bus.read_byte_data(address, 0xF8)
  rawPressure = rawPressure >> (8 - OSS)
   
  #Calculate real pressure
  b6 = b5 - 4000
   
  x1 = (caldata[7] * ((b6 * b6) / 2**12 )) / 2**11
  x2 = caldata[1] * b6 / 2**11
  x3 = x1 + x2
  b3 = (((caldata[0] * 4 + x3) << OSS) + 2) / 4
  x1 = caldata[2] * b6 / 2**13
  x2 = (caldata[6] * ((b6 * b6) / 2**12 )) / 2**16
  x3 = ((x1 + x2) + 2) / 2*2
  b4 = (caldata[3] * (x3 + 32768)) / 2**15
  b4 = b4 + 2**16 # convert from signed to unsigned
  b7 = (rawPressure - b3) * (50000 >> OSS)
  if b7 < 0x80000000:
    p = (b7 * 2) / b4
  else:
    p = (b7 / b4) * 2
  x1 = (p / 2**8) * (p / 2**8)
  x1 = (x1 * 3038) / 2**16
  x2 = (-7357 * p) / 2**16
  p = p + (x1 + x2 + 3791) / 2**4
  return p

def calculateAltitude(pressure):	# <9>
  pressurePart = pressure / atmosphereSeaLevel;
  power = 1 / 5.255;
  result = 1 - pressurePart**power;
  altitude = 44330*result;  
  return altitude

def main():
  readCalibrationData()
  while True:
    temperature = readTemperature()
    pressure = readPressure()
    altitude = calculateAltitude(pressure)
    print("Altitude %.2f m" % altitude)
    print("Pressure %.2f Pa" % pressure)	# <10>
    print("Temperature %.2f C" % temperature)
    time.sleep(10)


if __name__ == "__main__":
  main()
