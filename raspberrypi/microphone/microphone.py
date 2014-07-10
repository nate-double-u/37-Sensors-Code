# microphone.py - read sound from analog and print it
# (c) BotBook.com - Karvinen, Karvinen, Valtokari

import time
import botbook_mcp3002 as mcp	# <1>

def readSound(samples):
	buff = []	# <2>
	for i in range(samples):	# <3>
		buff.append(mcp.readAnalog())	# <4>
	return buff

def main():
	while True:
		sound = readSound(1024)	# <5>
		print(sound)
		time.sleep(1)	# s

if __name__ == "__main__":
	main()
