# human_body_touch_sensor.py - read touch information.
# (c) BotBook.com - Karvinen, Karvinen, Valtokari

import time
import botbook_gpio as gpio

def main():
	touch_pin = 25
	gpio.mode(touch_pin, "in")	
	while True:
		touchState = gpio.read(touch_pin)
		if( touchState == gpio.HIGH ):
			print("Touch detected")
		else:
			print("No touch detected")
		time.sleep(0.5)

if __name__ == "__main__":
	main()


