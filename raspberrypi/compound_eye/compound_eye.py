# compound_eye.py - read distance and direction.
# (c) BotBook.com - Karvinen, Karvinen, Valtokari

import time
import botbook_gpio as gpio	# <1>
import botbook_mcp3002 as mcp	# <2>

irUpValue = 0	# <3>
irDownValue = 0
irLeftValue = 0
irRightValue = 0
distance = 0

def readCompoundEye():
	global irUpValue,irDownValue,irLeftValue,irRightValue,distance	# <4>
	ledPin = 25	
	gpio.mode(ledPin, "out")	# <5>
	gpio.write(ledPin, gpio.HIGH)
	#Wait for sensors to get ready
	time.sleep(0.05)	# <6>
	
	irUpValue = mcp.readAnalog(0, 0)	# <7>
	irDownValue = mcp.readAnalog(1, 0)
	irLeftValue = mcp.readAnalog(0, 1)
	irRightValue = mcp.readAnalog(1, 1)
	
	ambientLight = 0
	gpio.write(ledPin, gpio.LOW)	# <8>
	time.sleep(0.05)
	ambientLight = mcp.readAnalog(0, 0)	# <8>
	irUpValue = irUpValue - ambientLight	# <9>
	ambientLight = mcp.readAnalog(1, 0)	# <10>
	irDownValue = irDownValue - ambientLight
	ambientLight = mcp.readAnalog(0, 1)
	irLeftValue = irLeftValue - ambientLight
	ambientLight = mcp.readAnalog(1, 1)
	irRightValue = irRightValue - ambientLight
	
	distance = (irUpValue+irDownValue+irLeftValue+irRightValue)/4	# <11>	

def main():
	global irUpValue,irDownValue,irLeftValue,irRightValue,distance
	while True:	# <12>
		readCompoundEye()	# <13>
		print "Values:"
		print "Up: %f" % irUpValue
		print "Down: %f" % irDownValue
		print "Left: %f" % irLeftValue
		print "Right: %f" % irRightValue
		print "Distance: %f" % distance
		time.sleep(0.5) # s	# <14>

if __name__ == "__main__":
	main()
