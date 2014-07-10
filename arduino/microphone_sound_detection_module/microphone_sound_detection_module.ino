// microphone_sound_detection_module.ino - use interrupt to head sound triggering
// (c) BotBook.com - Karvinen, Karvinen, Valtokari


const int analogPin = A0;
const int digitalPin = 0; //UNO,Mega pin 2, Leonardo pin 3
volatile int sensorState = -1;
int soundVolume = -1;

void setup() { 
  Serial.begin(115200);
  attachInterrupt(digitalPin,soundTriggered,RISING); 
}

void loop() 
{

  //Read microphone voltage = sound volume
  soundVolume = analogRead(analogPin);
  //Serial.print("Sound volume: ");
  Serial.println(soundVolume);
  if(sensorState == 1) {
      Serial.println("Sound triggered");
      sensorState = 0; // start waiting for next sound trigger.
  } else if (soundVolume > 50) {
      Serial.println("Sound detected");  
  }
  delay(10);
}

void soundTriggered() {
  sensorState = 1; //Sound treshold is broken.
}
