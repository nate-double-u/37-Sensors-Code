// follow_light_with_servo.ino - follow light with servo
// (c) BotBook.com - Karvinen, Karvinen, Valtokari

const int servoPin = 8;
const int leftSensorPin = 0;
const int rightSensorPin = 1;
const int centerPos = 1400; //This value depends on servo
const int leftPosition = 800; //Servo left and right limits
const int rightPosition = 2000;
int servoPosition = 1400;
int servoSpeed = 40; //How fast servo moves.

void setup() {
  Serial.begin(115200);
  pinMode(servoPin, OUTPUT); 
}

void loop() {
  int leftSensor,rightSensor;
  //Read light values.
  leftSensor = analogRead(leftSensorPin);
  rightSensor = analogRead(rightSensorPin);
  if(leftSensor > rightSensor) {
    servoPosition -= servoSpeed;
  } else {
    servoPosition += servoSpeed;    
  }
  //Limit check
  if(servoPosition > rightPosition) {
    servoPosition = rightPosition;  
  }
  if(servoPosition < leftPosition) {
    servoPosition = leftPosition;  
  }
  pulseServo(servoPin,servoPosition);
  delay(10);
}

void pulseServo(int pin, int microseconds)
{
  digitalWrite(pin, HIGH);
  delayMicroseconds(microseconds);
  digitalWrite(pin, LOW);
  delay(5);
}
