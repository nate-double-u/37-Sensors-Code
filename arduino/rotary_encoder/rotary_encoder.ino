// rotary_encoder.ino - print encoder position
// (c) BotBook.com - Karvinen, Karvinen, Valtokari

const int clkPin = 2;
const int dtPin = 4;

volatile unsigned int encoderPos = 0;	// <1>

void setup() 
{
  Serial.begin(115200);
  pinMode(clkPin, INPUT);
  digitalWrite(clkPin, HIGH); // pull up	// <2>
  pinMode(dtPin, INPUT);
  digitalWrite(dtPin, HIGH); // pull up
  
  attachInterrupt(0, processEncoder, CHANGE);	// <3>
}

void loop()
{
  Serial.println(encoderPos);
  delay(100);  
}

void processEncoder()	// <4>
{
  if(digitalRead(clkPin) == digitalRead(dtPin))	// <5>
  {
    encoderPos++;	// turning right
  } else {
    encoderPos--;
  }
}
