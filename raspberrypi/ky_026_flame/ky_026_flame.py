# ky_026_flame.py - report presence of IR light from flame to serial
# (c) BotBook.com - Karvinen, Karvinen, Valtokari

import time
import botbook_gpio as gpio	# <1>

def main():
	triggerPin = 25
	gpio.mode(triggerPin, "in")	# <2>
	flame = gpio.read(triggerPin)	# <3>
	if(flame == gpio.HIGH):	# <4>
		print "Flame detected"
	else:
		print "No flame detected"
	time.sleep(0.5)

if __name__ == "__main__":
	main()


