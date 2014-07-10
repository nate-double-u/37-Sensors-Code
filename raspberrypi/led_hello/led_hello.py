# led_hello.py - light a LED using Raspberry Pi GPIO
# (c) BotBook.com - Karvinen, Karvinen, Valtokari

import time	# <1>
import os

def writeFile(filename, contents):	# <2>
	with open(filename, 'w') as f:	# <3>
		f.write(contents)

# main

print "Blinking LED on GPIO 27 once..."		# <4>

if not os.path.isfile("/sys/class/gpio/gpio27/direction"):	# <5>
	writeFile("/sys/class/gpio/export", "27")	# <6>

time.sleep(0.1) 
writeFile("/sys/class/gpio/gpio27/direction", "out")	# <7>

writeFile("/sys/class/gpio/gpio27/value", "1")	# <8>
time.sleep(2)	# seconds	# <9>
writeFile("/sys/class/gpio/gpio27/value", "0") # <10>

