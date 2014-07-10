# microswitch.py - write to screen if switch is pressed or not
# (c) BotBook.com - Karvinen, Karvinen, Valtokari
import time
import botbook_gpio as gpio	# <1>

def main():
	switchpin = 3	# has internal pull-up	# <2>
	gpio.mode(switchpin, "in")

	while (True):
		switchState = gpio.read(switchpin)	# <3>
		if(switchState == gpio.LOW):
			print "Switch is pressed"
		else:
			print "Switch is up"
		time.sleep(0.3)	# seconds

if __name__ == "__main__":
	main()

