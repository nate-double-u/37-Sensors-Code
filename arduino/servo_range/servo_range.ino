// servo_range.ino - turn servo and print values to serial
// (c) BotBook.com - Karvinen, Karvinen, Valtokari
int servoPin=2;

void pulseServo(int servoPin, int pulseLenUs)	// <1>
{
	digitalWrite(servoPin, HIGH);	// <2>
	delayMicroseconds(pulseLenUs);	// <3>
	digitalWrite(servoPin, LOW);	// <4>
	delay(15);	// <5>
}

void setup()
{
	pinMode(servoPin, OUTPUT);
	Serial.begin(115200);
}

void loop()
{
	for (int i=500; i<=3000; i=i+2) {	// <6>
		pulseServo(servoPin, i);	// <7>
		Serial.println(i);
	}
}

