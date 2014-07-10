// piezo_beep.ino - beep on a given frequency with a piezo speaker
// (c) BotBook.com - Karvinen, Karvinen, Valtokari

int speakerPin = 10;
 
void wave(int pin, float frequency, int duration)	// <1>
{ 
  float period=1/frequency*1000*1000; // microseconds (us)	// <2>
  long int startTime=millis();	// <3>
  while(millis()-startTime < duration) {	// <4>
    digitalWrite(pin, HIGH);	// <5>
    delayMicroseconds(period/2);
    digitalWrite(pin, LOW);
    delayMicroseconds(period/2);
  }
}
 
void setup() 
{
 pinMode(speakerPin, OUTPUT);
}
 
void loop() 
{
  wave(speakerPin, 440, 500);	// <6>
  delay(500);
}
