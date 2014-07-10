# voltage_record.py - record voltage from solar cell and print history to png
# (c) BotBook.com - Karvinen, Karvinen, Valtokari

import time
import os
import matplotlib	# <1>
matplotlib.use("AGG")	# <2>
import matplotlib.pyplot as plt
import numpy
from datetime import datetime
from datetime import date
from datetime import timedelta
import attopilot_voltage	# <3>
import shelve

historyFile = "/home/pi/record"	# <4>
plotFile = "/home/pi/public_html/history.png"	# <5>

def measureNewDatapoint():
	return attopilot_voltage.readVoltage()	# <6>

def main():
	history = shelve.open(historyFile)	# <7>

	if not history.has_key("x"):	# <8>
		history["x"] = []	# <9>
		history["y"] = []

	history["x"] += [datetime.now()]	# <10>
	history["y"] += [measureNewDatapoint()]	# <11>

	now = datetime.now()	# <12>
	sampleCount = 24 * 60 / 5
	history['x'] = history["x"][-sampleCount:]
	history['y'] = history["y"][-sampleCount:]
	
	plot = plt.figure()	# <13>
	plt.title('Solar cell voltage over last 24h')
	plt.ylabel('Voltage V')
	plt.xlabel('Time')
	plt.setp(plt.gca().get_xticklabels(), rotation=35)
	plt.plot(history["x"], history["y"], color='#4884ff')	# <14>
	plt.savefig(plotFile)	# <15>
	history.close()	# <16>

if __name__ == '__main__':
	main()

