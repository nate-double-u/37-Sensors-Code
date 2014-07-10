// parallax_PIR_revA_cheating_pir.ino - light an LED when movement detected
// (c) BotBook.com - Karvinen, Karvinen, Valtokari



const int sensorPin = 8;
const int ledPin = 13;
int speakerPin = 10;
const int learningPeriod = 30*1000; // 30 seconds for learning period.
int learningCompleted = 0;

void setup() {
  Serial.begin(115200);
   pinMode(speakerPin, OUTPUT);
  pinMode(sensorPin, INPUT); 
  Serial.println("Start learning for next 30 seconds.");
  pinMode(ledPin, OUTPUT);
}

void alarm() 
{
  wave(speakerPin, 440, 40);
  delay(25);
  wave(speakerPin, 300, 20);
  wave(speakerPin, 540, 40);
  delay(25);
}

void wave(int pin, float frequency, int duration)
{ 
  float period=1/frequency*1000*1000; // microseconds (us)
  long int startTime=millis();
  while(millis()-startTime < duration) {
    digitalWrite(pin, HIGH);
    delayMicroseconds(period/2);
    digitalWrite(pin, LOW);
    delayMicroseconds(period/2);
  }
}

void loop() {
   if(millis() < learningPeriod) {
     return; // Sensor has not yet learned its environment.  
   }
   if(learningCompleted == 0) {
     learningCompleted = 1;
     Serial.println("Learning completed.");
   }
   if(digitalRead(sensorPin) == HIGH) {
     Serial.println("Movement detected");
     alarm();
     digitalWrite(ledPin, HIGH);
   } else {
     Serial.println("No movement detected");
     digitalWrite(ledPin, LOW);     
   }
   delay(100);
}
