# ldr.py - sense light level and print to screen
# (c) BotBook.com - Karvinen, Karvinen, Valtokari
import time
import botbook_mcp3002 as mcp

def main():
	while True:
		lightLevel = mcp.readAnalog()
		print("Current light level is %i " % lightLevel)
		time.sleep(0.5)	

if __name__ == "__main__":
	main()
