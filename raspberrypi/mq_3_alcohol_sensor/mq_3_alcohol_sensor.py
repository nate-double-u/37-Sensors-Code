# mq_3_alcohol_sensor.py - read digital output from alcohol sensor
# (c) BotBook.com - Karvinen, Karvinen, Valtokari

import time
import botbook_gpio as gpio

def readLimit():
  limitPin = 23
  gpio.setMode(limitPin,"in")
  return gpio.read(limitPin)
  

def main():
  while True:
    if readLimit() == gpio.HIGH:
      print("Limit breached!")
    time.sleep(0.5) 

if __name__ == "__main__":
  main()



