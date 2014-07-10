# fingerprint_scanner.py - learn and recognize fingerprints with GT-511C3
# (c) BotBook.com - Karvinen, Karvinen, Valtokari

import time
import serial
import struct

STX1 = 0x55	# <1>
STX2 = 0xAA

CMD_OPEN = 				0x01	# <2>
CMD_CLOSE = 			0x02
CMD_LED = 				0x12
CMD_GET_ENROLL_COUNT = 	0x20
CMD_ENROLL_START = 		0x22
CMD_ENROLL_1 = 			0x23
CMD_ENROLL_2 = 			0x24
CMD_ENROLL_3 = 			0x25
CMD_IS_FINGER_PRESSED = 0x26
CMD_DELETE_ALL = 		0x41
CMD_IDENTIFY = 			0x51
CMD_CAPTURE_FINGER = 	0x60

ACK = 0x30	# <3>
NACK = 0x31

port = None

def calcChecksum(package):	# <4>
	checksum = 0
	for byte in package:
		checksum += ord(byte)
	return int(checksum)

def sendCmd(cmd, param = 0):	# <5>
	package = chr(STX1)+chr(STX2)+struct.pack('<hih', 1, param, cmd)	# <6>
	checksum = calcChecksum(package)
	package += struct.pack('<h',checksum)	# <7>
	
	sent = port.write(package)

	if(sent != len(package)):
		print "Error communicating"
		return -1

	recv = port.read(sent)	# <8>
	recvPkg = struct.unpack('cchihh',recv)	# <9>

	if recvPkg[4] == NACK:
		print("error: %s" % recvPkg[3])
		return -2
	time.sleep(1)
	return recvPkg[3]

def startScanner():
	print("Open scanner communications")
	sendCmd(CMD_OPEN)

def stopScanner():
	print("Close scanner communications")
	sendCmd(CMD_CLOSE)
	

def led(status = True):
	if status:
		sendCmd(CMD_LED,1)
	else:
		sendCmd(CMD_LED,0)

def enrollFail():
	print("Enroll failed")
	led(False)
	stopScanner()

def identFail():
	print("Ident failed")
	led(False)
	stopScanner()	

def startEnroll(ident):
	sendCmd(CMD_ENROLL_START,ident)

def waitForFinger(state):
	if(state):
		while(sendCmd(CMD_IS_FINGER_PRESSED) == 0):
			time.sleep(0.1)
	else:
		while(sendCmd(CMD_IS_FINGER_PRESSED) > 0):
			time.sleep(0.1)

def captureFinger():
	return sendCmd(CMD_CAPTURE_FINGER)

def enroll(state):
	if state == 1:
		return sendCmd(CMD_ENROLL_1)
	if state == 2:
		return sendCmd(CMD_ENROLL_2)
	if state == 3:
		return sendCmd(CMD_ENROLL_3)				

def identifyUser():
	return sendCmd(CMD_IDENTIFY)

def getEnrollCount():
	return sendCmd(CMD_GET_ENROLL_COUNT)

def removeAll():
	return sendCmd(CMD_DELETE_ALL)

def main():
	print("Remove all identities from scanner")
	startScanner()
	removeAll()
	led()
	print("Start enroll")
	newID = getEnrollCount()
	print(newID)

	startEnroll(newID)
	print("Press finger to start enroll")
	waitForFinger(False)
	if captureFinger() < 0:
		enrollFail()
		return
	enroll(1)
	print("Remove finger")
	waitForFinger(True)


	print("Press finger again")
	waitForFinger(False)
	if captureFinger() < 0:
		enrollFail()
		return
	enroll(2)
	print("Remove finger")
	waitForFinger(True)

	print("Press finger again")
	waitForFinger(False)
	if captureFinger() < 0:
		enrollFail()
		return

	if enroll(3) != 0:
		enrollFail()
		return

	print("Remove finger")
	waitForFinger(True)

	print("Press finger again to identify")	
	waitForFinger(False)
	if captureFinger() < 0:	# <10>
		identFail()
		return
	ident = identifyUser()
	if(ident >= 0 and ident < 200):	# <11>
		print("Identity found: %d" % ident)
	else:
		print("User not found")
	led(False)	
	stopScanner()

if __name__ == "__main__":
	try:
		if port == None:
			port = serial.Serial(
				"/dev/ttyAMA0",
				baudrate=9600,
				timeout=None)	# <12>
		main()
	except Exception, e:
		print e
		port.close()
	finally:
		port.close()

