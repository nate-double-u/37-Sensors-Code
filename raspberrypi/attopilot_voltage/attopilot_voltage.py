# attopilot_voltage.py - measure current and voltage with Attopilot 13.6V/45A
# (c) BotBook.com - Karvinen, Karvinen, Valtokari
import time
import botbook_mcp3002 as mcp	# <1>

def readVoltage():
	raw = mcp.readAnalog(0,1)	# <2>
	percent = raw / 1023.0	# <3>
	volts = percent * 3.3	# <4>
	sensedVolts = volts * 13.6 / 3.3	# V/V	# <5>
	return sensedVolts	# V

def readCurrent():
	raw = mcp.readAnalog(0,0)
	percent = raw / 1023.0
	volts = percent * 3.3
	sensedCurrent = volts * 45.0 / 3.3	# A/V # <6>
	return sensedCurrent	# A

def main():
	while True:
		voltage = readVoltage()
		current = readCurrent()
		print("Current %.2f A" % current)
		print("Voltage %.2f V" % voltage)
		time.sleep(0.5) # s	

if __name__ == "__main__":
	main()

