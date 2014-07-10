// haunted_bell.ino - bell rings just before you touch it
// (c) BotBook.com - Karvinen, Karvinen, Valtokari



int servoPin=2;
int sensorPin = 9;
int sensorPowerPin = 8;
int hasRang = 0;	// <1>

void pulseServo(int servoPin, int pulseLenUs)	// <2>
{
  digitalWrite(servoPin, HIGH);
  delayMicroseconds(pulseLenUs);
  digitalWrite(servoPin, LOW);
  delay(20);
}

void cling()	// <3>
{
    for (int i=0; i<=3; i++) {	// <4>
      pulseServo(servoPin, 2000);
    }    
    for (int i=0; i<=100; i++) {	
      pulseServo(servoPin, 1000);
    }

}

void setup()
{
  pinMode(servoPin, OUTPUT);
  pinMode(sensorPowerPin, OUTPUT);
  digitalWrite(sensorPowerPin,HIGH);
  pinMode(sensorPin,INPUT);
}

void loop()
{
  int touch = digitalRead(sensorPin);	// <5>
  if(touch == HIGH) {	// <6>
    hasRang = 0; 
  }
  if(touch == LOW && hasRang == 0) {	// <7>
    cling();	// <8>
    hasRang = 1;	// <9>
    digitalWrite(sensorPowerPin,LOW);
    delay(1);
    digitalWrite(sensorPowerPin,HIGH);
  }
  delay(100);
}

