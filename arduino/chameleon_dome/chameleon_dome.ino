// chameleon_dome.ino - cube changes color to match the surface
// (c) BotBook.com - Karvinen, Karvinen, Valtokari

const int gsr1Pin = 7;	// <1>
const int gsr0Pin = 6;
const int gsg1Pin = 5;
const int gsg0Pin = 4;
const int gsb1Pin = 3;
const int gsb0Pin = 2;

const int ledPin = 8;	// <2>

const int redInput = A0;	// <3>
const int greenInput = A1;
const int blueInput = A2;

const int redOutput = 11;	// <4>
const int greenOutput = 10;
const int blueOutput = 9;

int red = -1;	// <5>
int green = -1;
int blue = -1;

const float newWeight = 0.7;	// <6>

void setup() {
  Serial.begin(115200);  
  pinMode(gsr1Pin, OUTPUT); 
  pinMode(gsr0Pin, OUTPUT);
  pinMode(gsg1Pin, OUTPUT);
  pinMode(gsg0Pin, OUTPUT);
  pinMode(gsb1Pin, OUTPUT);
  pinMode(gsb0Pin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(redOutput, OUTPUT);
  pinMode(greenOutput, OUTPUT);
  pinMode(blueOutput, OUTPUT); 
 
  digitalWrite(ledPin, HIGH);	// <7>
  
  digitalWrite(gsr1Pin, LOW); 
  digitalWrite(gsr0Pin, LOW);
  digitalWrite(gsg1Pin, LOW);
  digitalWrite(gsg0Pin, LOW);
  digitalWrite(gsb1Pin, LOW);
  digitalWrite(gsb0Pin, LOW);
}

void loop() {
  int redValue = analogRead(redInput);	// <8>
  int greenValue = analogRead(greenInput);
  int blueValue = analogRead(blueInput);

  redValue = redValue * 10 / 1.0;
  greenValue = greenValue * 10 / 0.75;
  blueValue = blueValue * 10 / 0.55;

  redValue = map(redValue, 0, 1023, 0, 255);	// <9>
  greenValue = map(greenValue, 0, 1023, 0, 255);
  blueValue = map(blueValue, 0, 1023, 0, 255); 

  if(redValue > 255) redValue = 255;	// <10>
  if(greenValue > 255) greenValue = 255;
  if(blueValue > 255) blueValue = 255;  

  red = runningAverage(redValue, red);	// <10>
  green = runningAverage(greenValue, green);
  blue = runningAverage(blueValue, blue);

  Serial.print(red); Serial.print(" ");
  Serial.print(green); Serial.print(" ");
  Serial.print(blue); Serial.println(" ");
  if(red < 200 || green < 180 || blue < 180) {
    green = green - red * 0.3;	// <11>
    blue = blue - red * 0.3;
  }

  red = easing(red);	// <12>
  green = easing(green);
  blue = easing(blue);

  setColor(red,green,blue);	// <13>
  
  delay(100);  
}

int runningAverage(int input, int old) {
  return newWeight*input + (1-newWeight)*old;	// <14>
}

int easing(int input) {	// <15>
  float percent = input / 255.0f;
  return 255.0f * percent * percent * percent * percent;
}

int setColor(int r, int g, int b) {	// <16>
  analogWrite(redOutput, 255-r);	// <17>
  analogWrite(greenOutput, 255-g);
  analogWrite(blueOutput, 255-b);  
}

