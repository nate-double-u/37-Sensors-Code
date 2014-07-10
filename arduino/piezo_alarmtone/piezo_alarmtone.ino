// piezo_alarmtone.ino - use piezospeaker to sound alarm sound
// (c) BotBook.com - Karvinen, Karvinen, Valtokari
 
int speakerPin = 10;
 
void wave(int pin, float frequency, int duration)	// <1>
{ 
  float period=1 / frequency * 1000 * 1000; // microseconds (us)
  long int startTime=millis();
  while(millis()-startTime < duration) {
    digitalWrite(pin, HIGH);
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
  wave(speakerPin, 440, 40);	// <2>
  delay(25);
  wave(speakerPin, 300, 20);
  wave(speakerPin, 540, 40);
  delay(25);
  wave(speakerPin, 440, 20);
  wave(speakerPin, 640, 40);
  delay(25);
  wave(speakerPin, 540, 20);
}
