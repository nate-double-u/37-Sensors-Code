# adjustable-infrared-sensor-switch.py - read infrared switch
# (c) BotBook.com - Karvinen, Karvinen, Valtokari
import time
import botbook_gpio as gpio	# <1>

def main():
	switchPin = 27
	gpio.mode(switchPin, "in")	# <2>
	x = gpio.read(switchPin)	# <3>
	if( x == gpio.LOW ):	# <4>
		print "Something is inside detection range"
	else:
		print "There is nothing inside detection range"
	time.sleep(0.1)

if __name__ == "__main__":
	main()
