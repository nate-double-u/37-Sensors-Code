# parallax_pir_reva.py - write to screen when movement detected
# (c) BotBook.com - Karvinen, Karvinen, Valtokari
import time
import botbook_gpio as gpio

learningPeriod = 60

def main():
	pirPin = 14	
	gpio.mode(pirPin, "in")	
	#Learning period
	time.sleep(learningPeriod) # <1>
	while (True):	
		movement = gpio.read(pirPin) # <2>	
		if(movement == gpio.HIGH):
			print "Movement detected"
		else:
			print "No movement detected"
		time.sleep(0.3)	

if __name__ == "__main__":
	main()
