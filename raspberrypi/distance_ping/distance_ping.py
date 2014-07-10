# distance_ping.py - print distance
# (c) BotBook.com - Karvinen, Karvinen, Valtokari
import time # <1>
import botbook_gpio as gpio # <2> 

def readDistanceCm():
	sigPin=22
	v=(331.5+0.6*20)
	
	gpio.interruptMode(sigPin, "both")	# <3>

	gpio.mode(sigPin, "out")	# <4>
	gpio.write(sigPin, gpio.LOW)	# <5>
	time.sleep(0.5)	# s

	gpio.write(sigPin, gpio.HIGH)	# <6>
	time.sleep(1/1000.0/1000.0)	# <7>
	gpio.mode(sigPin, "in")	# <8>

	#Read high pulse width
	t = gpio.pulseInHigh(sigPin) # s	# <9>
	d = t*v
	d = d/2 # <10>
	return d*100	# cm

def main():
	d = readDistanceCm()	# <11>
	print "Distance is %.2f cm" % d	# <12>
	time.sleep(0.5)

if __name__ == "__main__":
	main()

