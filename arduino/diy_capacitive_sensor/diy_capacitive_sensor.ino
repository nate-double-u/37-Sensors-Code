// diy_capacitive_sensor.ino - measure touch
// (c) BotBook.com - Karvinen, Karvinen, Valtokari

const int sendPin = 4;
const int readPin = 6;

void setup() {
   Serial.begin(115200); 
   pinMode(sendPin,OUTPUT);
   pinMode(readPin,INPUT);
   digitalWrite(readPin,LOW);
}

void loop() {
  int time = 0;

  digitalWrite(sendPin,HIGH);
  while(digitalRead(readPin) == LOW) time++;
  Serial.println(time);
  digitalWrite(sendPin,LOW);
  delay(100);  
}
