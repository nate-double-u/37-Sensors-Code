// parallax_pir_reva.ino - print movement detection
// (c) BotBook.com - Karvinen, Karvinen, Valtokari



const int sensorPin = 8;
const int ledPin = 13;
const int learningPeriod = 30*1000; // ms	// <1>
int learningCompleted = 0;

void setup() {
  Serial.begin(115200);
  pinMode(sensorPin, INPUT); 
  Serial.println("Starting to learn unmoving environment...");	// <2>
  pinMode(ledPin, OUTPUT);
}

void loop() {
   if(millis() < learningPeriod) {	// <3>
     delay(20); // ms	// <4>
     return;	// <5>
   }
   if(learningCompleted == 0) {	// <6>
     learningCompleted = 1;
     Serial.println("Learning completed.");
   }

   if(digitalRead(sensorPin) == HIGH) {	// <7>
     Serial.println("Movement detected");
     digitalWrite(ledPin,HIGH);
   } else {
     Serial.println("No movement detected");
     digitalWrite(ledPin,LOW);     
   }
   delay(100);
}
