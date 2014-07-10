# line_sensor.py - print to serial if we are on a line
# (c) BotBook.com - Karvinen, Karvinen, Valtokari
import time
import os
import botbook_gpio as gpio	# <1>

def main():
	linePin = 23
	gpio.mode(linePin, "in")
	while True:
		lineState = gpio.read(linePin)	# <2>
		if( lineState == gpio.HIGH ):
			print "Sensor is on the line"
		else:
			print "Sensor is off the line"
		time.sleep(0.5)

if __name__ == "__main__":
	main()



