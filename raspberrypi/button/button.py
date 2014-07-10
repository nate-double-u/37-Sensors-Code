# button.py - write to screen if button is down or up
# (c) BotBook.com - Karvinen, Karvinen, Valtokari

import time
import botbook_gpio as gpio	# <1>

def main():
	buttonpin = 3	# has internal pull-up	# <2>
	gpio.mode(buttonpin, "in")	# <3>

	while (True):	# <4>
		buttonUp = gpio.read(buttonpin)	# <5>
		if(buttonUp == gpio.HIGH):
			print "Button is up"
		else:
			print "Button is pressed"
		time.sleep(0.3)	# seconds	# <6>

if __name__ == "__main__":
	main()

