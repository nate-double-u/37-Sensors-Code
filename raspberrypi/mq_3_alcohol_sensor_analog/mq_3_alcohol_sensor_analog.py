# mq_3_alcohol_sensor_analog.py - read analog value from alcohol sensor and print.
# (c) BotBook.com - Karvinen, Karvinen, Valtokari

import time
import botbook_mcp3002 as mcp

def readAlcoholLevel():
  return mcp.readAnalog()
  
def main():
  while True:
    level = readAlcoholLevel()
    print("Current alcohol level is %i " % level)
    time.sleep(0.5) 

if __name__ == "__main__":
  main()



