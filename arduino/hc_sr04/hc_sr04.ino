// hc_sr04.ino - print distance to serial
// (c) BotBook.com - Karvinen, Karvinen, Valtokari

int trigPin = 8;
int echoPin = 7;
float v=331.5+0.6*20; // m/s

void setup()
{
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);	// <1>
  pinMode(echoPin, INPUT);	// <2>
}

float distanceM(){
  // send sound pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(3);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);

  // listen for echo 
  float tUs = pulseIn(echoPin, HIGH); // microseconds
  float t = tUs / 1000.0 / 1000.0 / 2; // s
  float d = t*v; // m
  return d*100; // cm
}

void loop()	// <3>
{
  int d=distanceM();
  Serial.println(d, DEC);
  delay(200); // ms
}

