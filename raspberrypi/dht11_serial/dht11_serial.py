# dht11_serial.py - print humidity and temperature using DHT11 sensor
# (c) BotBook.com - Karvinen, Karvinen, Valtokari

import time
import serial	# <1>

def main():
	port = serial.Serial("/dev/ttyACM0", baudrate=115200, timeout=None)	# <2>
	while True:
		line = port.readline()	# <3>
		arr = line.split()	# <4>
		if len(arr) < 3:	# <5>
			continue	# <6>
		dataType = arr[2]
		data = float(arr[1])	# <7>
		if dataType == '%':
			print("Humidity: %.1f %%" % data)
		else:
			print("Temperature: %.1f C" % data)


		time.sleep(0.01)

if __name__ == "__main__":
	main()

