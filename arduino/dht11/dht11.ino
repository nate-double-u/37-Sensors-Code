// dht11.ino - print humidity and temperature using DHT11 sensor
// (c) BotBook.com - Karvinen, Karvinen, Valtokari

const int dataPin = 8;

int temperature = -1;
int humidity = -1;

void setup() {
  Serial.begin(115200);
}

int readDHT11() {
  uint8_t recvBuffer[5];	// <1>
  uint8_t cnt = 7;
  uint8_t idx = 0;
  for(int i = 0; i < 5; i++) recvBuffer[i] = 0;	// <2>
  
  // Start communications with LOW pulse
  pinMode(dataPin, OUTPUT);
  digitalWrite(dataPin, LOW);
  delay(18);
  digitalWrite(dataPin, HIGH);

  delayMicroseconds(40);	// <3>
  pinMode(dataPin, INPUT);
  pulseIn(dataPin, HIGH);   // <4>
  // Read data packet
  unsigned int timeout = 10000; // loop iterations
  for (int i=0; i<40; i++)	// <5>
  {
          timeout = 10000; 
          while(digitalRead(dataPin) == LOW) {
                  if (timeout == 0) return -1;
                  timeout--;
          }

          unsigned long t = micros();	// <6>

          timeout = 10000;
          while(digitalRead(dataPin) == HIGH) {	// <7>
                  if (timeout == 0) return -1;
                  timeout--;
          }

          if ((micros() - t) > 40) recvBuffer[idx] |= (1 << cnt);	// <8>
          if (cnt == 0)   // next byte?
          {
                  cnt = 7;    // restart at MSB
                  idx++;      // next byte!
          }
          else cnt--;
  }

  humidity = recvBuffer[0];	// %	// <9>
  temperature = recvBuffer[2];	// C
  uint8_t sum = recvBuffer[0] + recvBuffer[2];
  if(recvBuffer[4] != sum) return -2;	// <10>
  return 0;
}

void loop() {
  int ret = readDHT11();
  if(ret != 0) Serial.println(ret);
  Serial.print("Humidity: "); Serial.print(humidity); Serial.println(" %");
  Serial.print("Temperature: "); Serial.print(temperature); Serial.println(" C"); 
  delay(2000); // ms
}

