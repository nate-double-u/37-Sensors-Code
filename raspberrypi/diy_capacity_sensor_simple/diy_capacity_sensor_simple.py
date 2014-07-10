# diy_capacity_sensor_simple.py - read touch from diy capacity sensor.
# (c) BotBook.com - Karvinen, Karvinen, Valtokari
import time
import botbook_gpio as gpio

def sample(count):
	sendPin = 23
	recievePin = 24
	gpio.mode(sendPin,"out")
	gpio.mode(recievePin,"in")
	gpio.write(sendPin,0)
	total = 0
	# set low
	for x in xrange(1,count):
		time.sleep(0.01)
		gpio.write(sendPin,gpio.HIGH)
		while(gpio.read(recievePin) == False):
			total += 1
		gpio.write(sendPin,gpio.LOW)
	return total
  

def main():
	while True:
		touch = sample(30)
		print("Touch: %d" % touch)
		time.sleep(0.5) 

if __name__ == "__main__":
	main()