# image2c-botbook - convert a BMP image to C for use in eInk and LCD displays
# (c) BotBook.com - Karvinen, Karvinen, Valtokari

import Image	# <1>
import math
imageName = "images/foo.bmp"	# <2>
outputName = "foo.h"	# <3>

im = Image.open(imageName)	# <4>
print im.format, im.size, im.mode

width, height = im.size
pixels = list(im.getdata())	# <5>
length = int(math.floor(width * height / 8.0))	# <6>
carray = length * [0x00]	# <7>
for y in xrange(0, height):	# <8>
	for x in xrange(0, width):	# <9>
		pixel = pixels[y * width + x]	# <10>
		
		bit = 7 - x & 0x07	# <11>
		byte = x / 8 + y * (width / 8)	# <12>

		mask = 0x01 << bit	# <13>
		if pixel == 0:
			carray[byte] |= mask	# <14>
		else:
			carray[byte] &= ~mask



fileHandle = open(outputName, "w")	# <15>
index = 0
fileHandle.write("// File generated with image2c-botbook.py\n")
fileHandle.write("const unsigned char sun [] PROGMEM= {\n")
for b in carray:
	fileHandle.write("0x%02X, " % b)	# <16>
	index += 1
	if index > 15:
		fileHandle.write("\n")
		index = 0
fileHandle.write("};\n")

