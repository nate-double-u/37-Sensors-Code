# botbook_gpio.py - gpio library.
# (c) BotBook.com - Karvinen, Karvinen, Valtokari

"""
Control GPIO trough kernel virtual file system /sys/, possibly without root. 

Unlike RPi.GPIO, tGPIO doesn't use kmem. This script only needs permissions
to /sys/class/gpio/ to use GPIO, no root access required. These permissions 
could be provided with a udev script. 
"""
import os.path
import time
import argparse
import sys
import subprocess
import select

HIGH = 1
LOW = 0

# Utility functions

def writeFile(file, contents):
	contents=str(contents)
	with open(file, 'wa') as f:
		f.write(contents)

def readFile(file):
	with open(file, 'r') as f:
		s=f.read()
	return s

def parseArgs():
	parser = argparse.ArgumentParser()
	parser.add_argument('--install-udev', action='store_true',
		help='Install udev rule to allow GPIO use without root (recommended)',)
	return parser.parse_args()

def chmodHelp():
	print "ERROR: not enough rights. You could run as root (not recommeneded). "
	print "To allow GPIO as normal user (recommended), run"
	print "	$ sudo ./botbook_gpio.py --install-udev"
	sys.exit(1)	

def installUdev():
	print "Installing udev rule..."	
	s ='# /etc/udev/rules.d/99-gpio.rules - GPIO without root on Raspberry Pi\n'
	s+='# (c) BotBook.com - Karvinen, Karvinen, Valtokari\n'
	s+='SUBSYSTEM=="gpio", ACTION=="add", '
	s+='RUN="/bin/bash -c '
	s+="'chown -R root.dialout /sys/class/gpio/ /sys/devices/virtual/gpio/; "
	s+="chmod g+s /sys/class/gpio/ /sys/devices/virtual/gpio/;" 
	s+="chmod -R ug+rw /sys/class/gpio/ /sys/devices/virtual/gpio/;';"
	s+='"\n'
	try:
		writeFile("/etc/udev/rules.d/99-gpio.rules", s)
	except IOError as e:
		if e.errno==13 and "Permission denied" in e.strerror:
			print "Install udev rule as root: "
			print "$ sudo ./botbook_gpio.py --install-udev"
			print "After that you can use GPIO without sudo. "
			sys.exit(1)
		else:
			raise e
	print "Reloading udev..."
	print subprocess.check_output("service udev reload", shell=True)
	print "Exporting pin 27 to trigger udev rule..."
	export(27)
	print "Done. You should be able to use GPIO without root now. "


# GPIO functions

def mode(pin, mode, force=False):
	assert 0<=pin<=27
	legalPins=[2, 3, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 22, 23, 24, 25, 27] # Raspberry Pi rev 2
	assert force or (pin in legalPins) # check pin or use dangerous: mode(88, "in", force=True)
	assert mode in ("in", "out")

	export(pin)

	try:
		writeFile("/sys/class/gpio/gpio%i/direction" % pin, mode)
	except IOError as e:
		if e.errno==13 and "Permission denied" in e.strerror:
			chmodHelp()
		else: 
			raise
def write(pin, state):
	assert 0<=pin<=27
	assert state in (1, 0)

	writeFile("/sys/class/gpio/gpio%i/value" % pin, state)

def read(pin): 
	assert 0<=pin<=27
	s = readFile("/sys/class/gpio/gpio%i/value" % pin)
	s = s[0]
	state = int(s)
	assert state in (1, 0)
	return state

def interruptMode(pin,mode):
	assert 0<=pin<=27
	assert mode in ("none", "rising", "falling", "both")
	export(pin)
	writeFile("/sys/class/gpio/gpio%i/edge" % pin, mode)	

def pulseInHigh(pin): 
	assert 0<=pin<=27
	
	with open("/sys/class/gpio/gpio%i/value" % pin, 'r') as f:
		po = select.poll()
		po.register(f, select.POLLPRI) #setup priority polling for more accurate time reading
		#Read high pulse width
		startTime = time.time()
		endTime = time.time()
		measureCycle = 0
		while True:
			events = po.poll(60000)
			f.seek(0)
			last_state = f.read()
			#We are reading high pulse width
			if last_state[0] == '0' and measureCycle == 1:
				endTime = time.time()
				break
			if last_state[0] == '1' and measureCycle == 0:
				startTime = time.time()
				measureCycle = 1
			if len(events) == 0:
				raise Exception('timeout')
		return endTime - startTime
	
	
def export(pin):
	"Pins are automatically exported when setting mode(), so user doesn't need to call export() directly."
	assert 0<=pin<=27
	if os.path.isfile("/sys/class/gpio/gpio%i/direction" % pin):
		return # already exported

	try:
		writeFile("/sys/class/gpio/export", pin)
	except IOError as e:
		if e.errno in (2, 13): # 13-permission-denied, 2-no-such-file
			chmodHelp()
		else: 
			raise
	time.sleep(0.5) # wait for udev rule to fix permissions, seems to work with 0.1 s

# Sample program (when running with ./tgpio)

def writeSample():
	print "Blinking LED 27 once..."

	mode(27, "out")

	write(27, 1)
	time.sleep(1)
	write(27, 0)

def main():
	opt=parseArgs()
	if opt.install_udev:
		installUdev()
		sys.exit()
	writeSample()

if __name__ == "__main__":
	main()

