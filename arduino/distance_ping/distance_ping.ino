// distance_ping.ino - distance using ultrasonic ping sensor
// (c) BotBook.com - Karvinen, Karvinen, Valtokari  

int pingPin = 2;
float v=331.5+0.6*20; // m/s  // <1>

void setup()
{
  Serial.begin(115200);
}

float distanceCm(){
  // send sound pulse
  pinMode(pingPin, OUTPUT); // <2>
  digitalWrite(pingPin, LOW);
  delayMicroseconds(3); // <3>
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5); // <4>
  digitalWrite(pingPin, LOW);

  // listen for echo
  pinMode(pingPin, INPUT); 
  float tUs = pulseIn(pingPin, HIGH); // microseconds  // <5>
  float t = tUs / 1000.0 / 1000.0 / 2; // s  // <6>
  float d = t*v; // m  // <7>
  return d*100; // cm
}

void loop()
{
  int d=distanceCm();	// <8>
  Serial.println(d, DEC);	// <9>
  delay(200); // ms	// <10>
}

