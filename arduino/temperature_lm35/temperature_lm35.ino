// temperature_lm35.ino - measure temperature (Celsius) with LM35 and print it
// (c) BotBook.com - Karvinen, Karvinen, Valtokari


int lmPin = A0;

void setup()
{
  Serial.begin(115200);
  pinMode(lmPin, INPUT);
}

float tempC()
{
  float raw = analogRead(lmPin);  // <1>
  float percent = raw/1023.0; // <2>
  float volts = percent*5.0; // <3>
  return 100.0*volts;  // <4>
}

void loop()
{
  Serial.println(tempC());
  delay(200); // ms
}

