// posture_alarm.ino - sound an alarm when IR switch detects bad posture
// (c) BotBook.com - Karvinen, Karvinen, Valtokari

int speakerPin = 10;
const int sensorPin = 2;
int switchState = 0;
 
void wave(int pin, float frequency, int duration)	// <1>
{ 
  float period=1/frequency*1000*1000; // microseconds (us)
  long int startTime=millis();
  while(millis()-startTime < duration) {
    digitalWrite(pin, HIGH);
    delayMicroseconds(period/2);
    digitalWrite(pin, LOW);
    delayMicroseconds(period/2);
  }
}

void alarm() // <2>
{
  wave(speakerPin, 440, 40);
  delay(25);
  wave(speakerPin, 300, 20);
  wave(speakerPin, 540, 40);
  delay(25);
  wave(speakerPin, 440, 20);
  wave(speakerPin, 640, 40);
  delay(25);
  wave(speakerPin, 540, 20);
}

void setup() 
{
  pinMode(speakerPin, OUTPUT);
  Serial.begin(115200);  
  pinMode(sensorPin, INPUT);
}
 
void loop() 
{
  switchState = digitalRead(sensorPin);
  Serial.println(switchState,BIN);
  if (switchState==0) {	// <3>
    alarm(); // <4>
  }
  delay(10);
}
