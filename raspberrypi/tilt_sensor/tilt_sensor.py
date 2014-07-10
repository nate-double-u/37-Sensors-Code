# tilt_sensor.py - print if sensor was tilted
# (c) BotBook.com - Karvinen, Karvinen, Valtokari

import time
import botbook_gpio as gpio	

def main():
	tiltpin = 3	# has internal pull-up	# <1>
	gpio.mode(tiltpin, "in")	

	while (True):	
		isNotTilted = gpio.read(tiltpin)	# <2>
		if(isNotTilted == gpio.LOW):
			print "Sensor is tilted"
		else:
			print "Sensor is not tilted"
		time.sleep(0.3)	# seconds	

if __name__ == "__main__":
	main()

