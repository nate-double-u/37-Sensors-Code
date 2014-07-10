# temperature_lm35.py - print temperature
# (c) BotBook.com - Karvinen, Karvinen, Valtokari
import time
import botbook_mcp3002 as mcp	# <1>
#LM35 data pin voltage keeps under 2V so no 
#level conversion needed
def main():
	while True:
		rawData = mcp.readAnalog()	# <2>
		percent = rawData / 1023.0	# <3>
		volts = percent * 3.3 	# <4>
		temperature = 100.0 * volts	# <5>
		print("Current temperature is %.1f C " % temperature)
		time.sleep(0.5)	

if __name__ == "__main__":
	main()

