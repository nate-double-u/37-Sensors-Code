# mq_x_smoke_sensor.py - print smoke level
# (c) BotBook.com - Karvinen, Karvinen, Valtokari
import time
import botbook_mcp3002 as mcp	# <1>

smokeLevel = 0

def readSmokeLevel():
	global smokeLevel
	smokeLevel = mcp.readAnalog()	# <2>

def main():
  while True:	# <3>
    readSmokeLevel()	# <4>
    print("Current smoke level is %i " % smokeLevel)	# <5>
    if smokeLevel > 120:
      print("Smoke detected")
    time.sleep(0.5) # s

if __name__ == "__main__":
  main()




