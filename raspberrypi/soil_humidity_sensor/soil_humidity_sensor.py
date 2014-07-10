# soil_humidity_sensor.py - read soil humidity by measuring its resistance.
# (c) BotBook.com - Karvinen, Karvinen, Valtokari

import time
import botbook_mcp3002 as mcp

def main():
	while True:
		h = mcp.readAnalog()	# <1>
		h = h / 1024 * 100	# <2>
		print("Current humidity is %d %%" % h)
		time.sleep(5) 

if __name__ == "__main__":
	main()

