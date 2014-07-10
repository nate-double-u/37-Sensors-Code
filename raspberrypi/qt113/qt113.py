# qt113.py - read touch information from QT113
# (c) BotBook.com - Karvinen, Karvinen, Valtokari

import time
import botbook_gpio as gpio

def main():
	limitPin = 23
	gpio.setMode(limitPin, "in")
	while True:
		if gpio.read(limitPin) == gpio.LOW:	# <1>
			print("Touch detected!")
		time.sleep(0.5) 

if __name__ == "__main__":
  main()
