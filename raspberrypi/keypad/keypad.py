# keypad.py - read 16-key numeric keypad (dx.com sku 149608)
# (c) BotBook.com - Karvinen, Karvinen, Valtokari


import time
import botbook_gpio as gpio	# <1>

keymap = []
keymap.append(['1', '2', '3', 'A'])	# <2>
keymap.append(['4', '5', '6', 'B'])
keymap.append(['7', '8', '9', 'C'])
keymap.append(['*', '0', '#', 'D'])

columns = [2, 3, 14, 15]	# <3>
rows = [18, 17, 27, 22]
lastReadTime = None
bounceTime = 0.03 # s

def initializeKeyPad():
	for x in range(len(rows)):
		gpio.mode(rows[x], 'in')
		gpio.mode(columns[x], 'in')

def getKeyPress():
	global lastReadTime
	foundKey = None
	if((time.time() - lastReadTime) > bounceTime):	# <4>
		#pulse columns and read pins
		for c in range(len(columns)):
			gpio.mode(columns[c], 'out')
			gpio.write(columns[c], gpio.LOW)	# <5>

			for r in range(len(rows)):
				if gpio.read(rows[r]) == gpio.LOW:	# <6>
					foundKey = keymap[r][c]	# <7>

			gpio.write(columns[c], gpio.HIGH)
			gpio.mode(columns[c], 'in')	# <8>
			if not foundKey == None:
				break	# <9>
		lastReadTime = time.time()
	return foundKey

def main():
	global lastReadTime
	initializeKeyPad()
	lastReadTime = time.time()
	while True:
		key = getKeyPress()
		if not key == None:
			print(key)
		time.sleep(0.1)	# s

if __name__ == "__main__":
	main()

