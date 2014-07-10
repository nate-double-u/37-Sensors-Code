# pot.py - potentiometer with mcp3002
# (c) BotBook.com - Karvinen, Karvinen, Valtokari
import spidev # installation help in book and botbook_mcp3002.py	# <1>
import time

def readPotentiometer():
	spi = spidev.SpiDev()	# <2>
	spi.open(0, 0)	# <3>
	command = [1, 128, 0]	# <4>
	reply = spi.xfer2(command)	# <5>
	#Parse reply 10 bits from 24 bit package
	data = reply[1] & 31	# <6>
	data = data << 6	# <7>
	data = data + (reply[2] >> 2)	# <8>
	spi.close()	# <9>
	return data

def main():
	while True:
		potentiometer = readPotentiometer()	# <10>
		print("Current potentiometer value is %i " % potentiometer)
		time.sleep(0.5)	

if __name__ == "__main__":
	main()
