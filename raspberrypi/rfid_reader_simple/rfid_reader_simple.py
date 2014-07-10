# rfid_reader_simple.py - read 125 kHz RFID tags with ELB149C5M electronic brick without checking checksum
# (c) BotBook.com - Karvinen, Karvinen, Valtokari

import time
import serial	# 
import struct

port = None
card = "3E00FB788D30"
def main():
	inputData = ""
	while True:
		recv = port.read()
		if ord(recv) == 0x02:
			inputData = ""
		elif ord(recv) == 0x03:
			if inputData == card:
				print "Valid card"
			else:
				print "Wrong card. go away!"
		else:
			inputData += recv


if __name__ == "__main__":
	if port == None:
		port = serial.Serial("/dev/ttyAMA0", baudrate=9600, timeout=None)
	main()