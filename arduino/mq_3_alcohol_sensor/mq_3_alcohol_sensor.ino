// mq_3_alcohol_sensor.ino - print alcohol value and limit digital info.
// (c) BotBook.com - Karvinen, Karvinen, Valtokari


const int analogPin = A0;
const int digitalPin = 8;

int limit = -1;
int value = 0;

void setup() { 
  Serial.begin(115200);
  pinMode(digitalPin,INPUT);  
}

void loop() 
{
  //Read analog value
  value = analogRead(analogPin);
  //Check if alcohol limit is breached
  limit = digitalRead(digitalPin);
  Serial.print("Alcohol value: ");
  Serial.print(value);
  Serial.print(" Limit: ");
  Serial.println(limit);
  delay(100);
}

