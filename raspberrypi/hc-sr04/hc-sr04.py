# hc-sr04.py - print distance to object in cm
# (c) BotBook.com - Karvinen, Karvinen, Valtokari
import time
import botbook_gpio as gpio

def readDistanceCm():
	triggerPin = 22	# <1>
	echoPin = 27

	v=(331.5+0.6*20) # m/s

	gpio.mode(triggerPin, "out")

	gpio.mode(echoPin, "in")
	gpio.interruptMode(echoPin, "both")

	gpio.write(triggerPin, gpio.LOW)
	time.sleep(0.5)

	gpio.write(triggerPin, gpio.HIGH)
	time.sleep(1/1000.0/1000.0)
	gpio.write(triggerPin, gpio.LOW)

	t = gpio.pulseInHigh(echoPin) # s

	d = t*v
	d = d/2
	return d*100	# cm

def main():
	d = readDistanceCm()	# <2>
	print "Distance is %.2f cm" % d
	time.sleep(0.5)

if __name__ == "__main__":
	main()

