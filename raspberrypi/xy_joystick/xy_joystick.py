# xy_joystick.py - print KY 023 joystick tilt and button status
# (c) BotBook.com - Karvinen, Karvinen, Valtokari
import time
import botbook_mcp3002  as mcp	# <1>
import botbook_gpio as gpio 	# <2>

def readX():	# <3>
	return mcp.readAnalog(0, 0)

def readY():
	return mcp.readAnalog(0, 1)	# <4>

def readButton():
	buttonPin = 25
	gpio.mode(buttonPin, "in")
	return gpio.read(buttonPin)

def main():
	while True:	# <5>
		xAxel = readX()	# <6>
		yAxel = readY()
		button = readButton()
		print("X: %i, Y: %i, Button: %r" % (xAxel, yAxel, button))	# <7>
		time.sleep(0.5) 

if __name__ == "__main__":
	main()

