// ky_023_xyjoystick.ino - print joystick position to serial
// (c) BotBook.com - Karvinen, Karvinen, Valtokari


const int VRxPin = 0;	// <1>
const int VRyPin = 1;
const int SwButtonPin = 8;

int button = -1; // LOW or HIGH	// <2>
int x = -1; // 0..1023
int y = -1; // 0..1023

void readJoystick() { // <3>
  button = digitalRead(SwButtonPin);	// <4>
  x = analogRead(VRxPin);
  y = analogRead(VRyPin); 
}

void setup()  { 
  pinMode(SwButtonPin, INPUT); 
  digitalWrite(SwButtonPin, HIGH); // pull-up resistor	// <5>
  Serial.begin(115200);
} 
 
void loop()  {
  readJoystick();	// <6>
  Serial.print("X: ");
  Serial.print(x);
  Serial.print(" Y: ");
  Serial.print(y);
  Serial.print(" Button: ");
  Serial.println(button); 
  delay(10);
}

