 // hellorgb.ino - mix colors with RGB LED
 // (c) BotBook.com - Karvinen, Karvinen, Valtokari

const int redPin=11;	// <1>
const int greenPin=10;
const int bluePin=9;

void setup()
{
        pinMode(redPin, OUTPUT);  
        pinMode(greenPin, OUTPUT);
        pinMode(bluePin, OUTPUT);
}

void loop()
{
        setColor(255, 0, 0);	// <2>
        delay(1000); 

        setColor(255, 255, 255);
        delay(1000);
}

void setColor(int red, int green, int blue)
{
        analogWrite(redPin, 255-red);	// <3>
        analogWrite(greenPin, 255-green);
        analogWrite(bluePin, 255-blue);
}
