# vibration_sensor.py - detect vibration
# (c) BotBook.com - Karvinen, Karvinen, Valtokari
import time
import botbook_gpio as gpio	# <1>

def main():
	vibPin = 3
	gpio.mode(vibPin, "in")	
	while (True):
		vibrationInput = gpio.read(vibPin)	# <2>
		if(vibrationInput == gpio.LOW):
			print "Vibration"
			time.sleep(5)	# <3>
		time.sleep(0.01) # seconds	# <4>

if __name__ == "__main__":
	main()
