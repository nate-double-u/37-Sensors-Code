# rotary_encoder.py - read rotary encoder
# (c) BotBook.com - Karvinen, Karvinen, Valtokari
import time
import botbook_gpio as gpio	# <1>

def main():
	encoderClk = 3
	encoderDt = 2
	gpio.mode(encoderClk, "in")
	gpio.mode(encoderDt, "in")
	encoderLast = gpio.LOW
	encoderPos = 0
	lastEncoderPos = 0
	while True: 
		clk = gpio.read(encoderClk)
		if encoderLast == gpio.LOW and clk == gpio.HIGH:	# <2>
			if(gpio.read(encoderDt) == gpio.LOW):	# <3>
				encoderPos += 1
			else:
				encoderPos -= 1
		encoderLast = clk
		if encoderPos != lastEncoderPos:
			print("Encoder position %d" % encoderPos)
		lastEncoderPos = encoderPos
		time.sleep(0.001) # s	# <4>

if __name__ == "__main__":
	main()

