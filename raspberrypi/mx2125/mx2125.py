# mx2125.py - print acceleration axel values.
# (c) BotBook.com - Karvinen, Karvinen, Valtokari
import time 
import botbook_gpio as gpio

xPin = 24
yPin = 23

def readAxel(pin):
	gpio.mode(pin, "in")
	gpio.interruptMode(pin, "both")
	return gpio.pulseInHigh(pin)	# <1>

def main():
	x_g = 0
	y_g = 0
	while True:
		x = readAxel(xPin) * 1000	
		y = readAxel(yPin) * 1000
		if(x < 10):	# <2>
			x_g = ((x / 10) - 0.5) * 8	# <3>
		if(y < 10):
			y_g = ((y / 10) - 0.5) * 8
		print ("Axels x: %fg, y: %fg" % (x_g, y_g))	#
		time.sleep(0.5)

if __name__ == "__main__":	
	main()
