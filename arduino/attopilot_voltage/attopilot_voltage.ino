// attopilot_voltage.ino - measure current and voltage with Attopilot 13.6V/45A
// (c) BotBook.com - Karvinen, Karvinen, Valtokari


int currentPin = A0;
int voltagePin = A1;

void setup()
{
  Serial.begin(115200);
  pinMode(currentPin, INPUT);
  pinMode(voltagePin, INPUT);
}

float current()
{
  float raw = analogRead(currentPin); 
  Serial.println(raw);
  float percent = raw/1023.0;	// <1>
  float volts = percent*5.0;	// <2>
  float sensedCurrent = volts * 45 / 3.3;	// A/V	// <3>
  return sensedCurrent; // A	// <4>
}

float voltage()
{
  float raw = analogRead(voltagePin);  
  float percent = raw/1023.0; 
  float volts = percent*5.0; 
  float sensedVolts = volts * 13.6 / 3.3;	// V/V	// <5>
  return sensedVolts;	// V
}

void loop()
{
  Serial.print("Current: ");
  Serial.print(current(),4);
  Serial.println(" A");
  Serial.print("Voltage: ");
  Serial.print(voltage());
  Serial.println(" V");
  delay(200); // ms
}

