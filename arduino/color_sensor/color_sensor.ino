// color_sensor.ino - sense color with HDJD-S822-QR999 and print RGB value
// (c) BotBook.com - Karvinen, Karvinen, Valtokari


const int gsr1Pin = 7;	// <1>
const int gsr0Pin = 6;
const int gsg1Pin = 5;
const int gsg0Pin = 4;
const int gsb1Pin = 3;
const int gsb0Pin = 2;

const int ledPin = 8;	// <2>

const int redPin = A0;	// <3>
const int greenPin = A1;
const int bluePin = A2;

int red = -1;	// <4>
int green = -1;
int blue = -1;

void setup() {
  Serial.begin(115200);
  pinMode(gsr1Pin, OUTPUT); 
  pinMode(gsr0Pin, OUTPUT);
  pinMode(gsg1Pin, OUTPUT);
  pinMode(gsg0Pin, OUTPUT);
  pinMode(gsb1Pin, OUTPUT);
  pinMode(gsb0Pin, OUTPUT);
  pinMode(ledPin, OUTPUT);  

  digitalWrite(ledPin, HIGH);	// <5>
  
  digitalWrite(gsr1Pin, LOW);   // <6>
  digitalWrite(gsr0Pin, LOW);
  digitalWrite(gsg1Pin, LOW);
  digitalWrite(gsg0Pin, LOW);
  digitalWrite(gsb1Pin, LOW);
  digitalWrite(gsb0Pin, LOW);
}

void loop() {
  int redValue = analogRead(redPin);	// <7>
  int greenValue = analogRead(greenPin);
  int blueValue = analogRead(bluePin);

  redValue = redValue * 10 / 1.0;  // <8>
  greenValue = greenValue * 10 / 0.75;
  blueValue = blueValue * 10 / 0.55;

  Serial.print(redValue); Serial.print(" ");	// <9>
  Serial.print(greenValue); Serial.print(" ");
  Serial.print(blueValue); Serial.println(" ");
  delay(100);  
}

