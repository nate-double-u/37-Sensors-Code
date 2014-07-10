# rfid_reader.py - read 125 kHz RFID tags with ELB149C5M electronic brick
# (c) BotBook.com - Karvinen, Karvinen, Valtokari

import time
import serial	# <1>
import struct

port = None

def main():
	global port
	bytesRead = -1	# <2>
	buff = [0x00]*12	# <3>
	print("Ready to receive tag")
	while True:	# <4>
		recv = port.read()	# <5>
		if(ord(recv) == 0x02):	# <6>
			bytesRead = 0
			print("Start reading tag")
		elif(bytesRead == 12 and ord(recv) == 0x03):	# <7>
			print("Checking tag")
			data = ""	# <8>
			checksum  = 0x00
			for x in 0, 2, 4, 6, 8, 10:
				hexString = ''.join( buff[ x : x+2 ] )	# <9>
				translatedByte = int(hexString, 16)
				data += chr(translatedByte)	# <10>
				checksum = checksum ^ translatedByte	# <11>
			cardData = struct.unpack(">cic", data)	# <12>

			if checksum != 0:
				print "Checksum calculation failed"
			print cardData[1]	# <13>
		else:	# <14>
			buff[bytesRead] = recv	# <15>
			bytesRead += 1


if __name__ == "__main__":
	if port == None:
		port = serial.Serial("/dev/ttyAMA0", baudrate=9600, timeout=None)
		port.flushInput()	
	main()
