# color_sensor.py - sense color and print RGB value to serial
# (c) BotBook.com - Karvinen, Karvinen, Valtokari

import time
import botbook_mcp3002 as mcp	# <1>
import botbook_gpio as gpio

def initializeColorSensor():
	ledPin = 25
	gpio.mode(2,"out")	# <2>
	gpio.mode(3,"out")
	gpio.mode(14,"out")
	gpio.mode(17,"out")
	gpio.mode(22,"out")
	gpio.mode(27,"out")
	
	gpio.write(2,gpio.LOW)
	gpio.write(3,gpio.LOW)
	gpio.write(14,gpio.LOW)
	gpio.write(17,gpio.LOW)
	gpio.write(22,gpio.LOW)
	gpio.write(27,gpio.LOW)

	gpio.mode(ledPin,"out")
	gpio.write(ledPin, gpio.HIGH)	# <3>

def main():
	initializeColorSensor()
	while True:	# <4>
		redValue = mcp.readAnalog(0, 0)
		greenValue = mcp.readAnalog(0, 1)
		blueValue = mcp.readAnalog(1, 0)	# <5>

		redValue = redValue * 10 / 1.0;	# <6>
		greenValue = greenValue * 10 / 0.75;
		blueValue = blueValue * 10 / 0.55;

		print("R: %d, G: %d, B: %d" % (redValue,greenValue,blueValue))	# <7>

		time.sleep(0.1)	# s

if __name__ == "__main__":
	main()

