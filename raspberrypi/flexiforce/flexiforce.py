# flexiforce.py - sense force and print value to screen.
# (c) BotBook.com - Karvinen, Karvinen, Valtokari
import time
import botbook_mcp3002 as mcp # <1>

def readFlexiForce():
	return mcp.readAnalog()	# <2>

def main():
	while True:	# <3>
		f = readFlexiForce()	# <4>
		print("Current force is %i " % f)	# <5>
		time.sleep(0.5) # s	# <6>

if __name__ == "__main__":
	main()

