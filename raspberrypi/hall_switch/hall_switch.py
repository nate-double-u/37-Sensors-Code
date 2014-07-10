# hall_switch.py - write to screen if magnet triggers the switch
# (c) BotBook.com - Karvinen, Karvinen, Valtokari
import time
import botbook_gpio as gpio	# <1>

def main():
	switchPin = 3	# has internal pull-up	# <2>
	gpio.mode(switchPin, "in")

	while (True):
		switchState = gpio.read(switchPin)	# <3>
		if(switchState == gpio.LOW):
			print "switch triggered"
		
		time.sleep(0.3)

if __name__ == "__main__":
	main()

