# hall_sensor.py - print raw value and magnets pole
# (c) BotBook.com - Karvinen, Karvinen, Valtokari
import time
import botbook_mcp3002 as mcp	# <1>

zeroLevel = 388	# <2>  

def main():
  while True:	
 	rawMagneticStrength = mcp.readAnalog()
	print("Raw strength:  %i " % rawMagneticStrength)	
	zeroedStrength = rawMagneticStrength - zeroLevel
	print("Zeroed strength: %i " % zeroedStrength)	
	if(zeroedStrength > 0):
		print("South pole") 
	elif(zeroedStrength < 0):
		print("North pole") 
	time.sleep(0.5) 

if __name__ == "__main__":
  main()




