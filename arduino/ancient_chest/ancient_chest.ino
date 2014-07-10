// ancient_chest.ino - fingerprint unlocks chest 
// (c) BotBook.com - Karvinen, Karvinen, Valtokari

const byte STX1 = 0x55;
const byte STX2 = 0xAA;

const word CMD_OPEN = 0x01;
const word CMD_CLOSE = 0x02;
const word CMD_LED = 0x12;
const word CMD_GET_ENROLL_COUNT = 0x20;
const word CMD_ENROLL_START = 0x22;
const word CMD_ENROLL_1 = 0x23;
const word CMD_ENROLL_2 = 0x24;
const word CMD_ENROLL_3 = 0x25;
const word CMD_IS_FINGER_PRESSED = 0x26;
const word CMD_DELETE_ALL = 0x41;
const word CMD_IDENTIFY = 0x51;
const word CMD_CAPTURE_FINGER = 0x60;

const word ACK = 0x30;
const word NACK = 0x31;

struct package {
  byte header1;
  byte header2;  
  word deviceID;
  unsigned long param;
  word cmd;
  word checksum;
};

const int SIZE_OF_PACKAGE = 12;

const int lockPin = 8;
const int resetButtonPin = 7;
const int addButtonPin = 6;
const int speakerPin = 10;

float lowPeep = 220;
float highPeep = 440;
int closed = 2000;
int opened = 1000;
int state = 0;

word calcChecksum(struct package *pkg) {
  word checksum = 0;
  byte *buffer = (byte*)pkg;
  for(int i=0; i < (sizeof(struct package) - sizeof(word)); i++) 
  {
    checksum += buffer[i];  
  }
  return checksum;
}

int sendCmd(word cmd, int param) {
  struct package pkg;
  pkg.header1 = STX1;
  pkg.header2 = STX2;
  pkg.deviceID = 1;
  pkg.param = param;
  pkg.cmd = cmd;
  pkg.checksum = calcChecksum(&pkg);
  byte *buffer = (byte*)&pkg;

  int bytesSent = Serial3.write(buffer, sizeof(struct package));

  if(bytesSent != sizeof(struct package)) {
    Serial.println("Error communicating");
    return -1; 
  }

  int bytesReceived = 0;
  char recvBuffer[SIZE_OF_PACKAGE];
  struct package *recvPkg = (struct package*) recvBuffer;
 
  
  bytesReceived = Serial3.readBytes(recvBuffer, sizeof(struct package));
  if(bytesReceived != SIZE_OF_PACKAGE) {
    Serial.println("Error communicating");
    return -1;
  }  

  if( recvPkg->header1 != STX1 || recvPkg->header2 != STX2) {
    Serial.println("Header error!");
    return -1; 
  }

  if(recvPkg->checksum != calcChecksum(recvPkg)) {
    Serial.println("Checksum mismatch error!");
    return -1;    
  }
  if(recvPkg->cmd == NACK) {
    Serial.println("NACK - Cmd error!");
    Serial.print("Error: ");
    Serial.println(recvPkg->param,HEX);
    return -1;     
  }

  return recvPkg->param;  
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

void pulseServo(int servoPin, int pulseLenUs)
{
  digitalWrite(servoPin, HIGH);	
  delayMicroseconds(pulseLenUs);	
  digitalWrite(servoPin, LOW);	
  delay(15);	
}

void peep(int count, float frequency) 
{
  for(int i = 0; i < count; i++) {
    wave(speakerPin, frequency, 400);
    delay(400); 
  }
}

void enrollFinger() {
  int id = 0;
  int ret = 0;
  id = sendCmd(CMD_GET_ENROLL_COUNT, 0);
  sendCmd(CMD_ENROLL_START, id);
  peep(1,lowPeep);
  WaitForFinger(false);
  ret = sendCmd(CMD_CAPTURE_FINGER, 1);	
  if(ret < 0) {
    peep(5,highPeep);
    return;  
  }  
  sendCmd(CMD_ENROLL_1, 0);
  peep(1,highPeep);
  WaitForFinger(true);
  
  WaitForFinger(false);
  ret = sendCmd(CMD_CAPTURE_FINGER, 1);	
  if(ret < 0) {
    peep(5,highPeep);
    return;  
  }  
  sendCmd(CMD_ENROLL_2, 0);
  peep(2,highPeep);
  WaitForFinger(true);
  
  WaitForFinger(false);
  ret = sendCmd(CMD_CAPTURE_FINGER, 1);	
  if(ret < 0) {
    peep(5,highPeep);
    return;  
  }  
  sendCmd(CMD_ENROLL_3, 0);
  peep(3,highPeep);
  WaitForFinger(true);  
}

void WaitForFinger(bool bePressed) {
  delay(500);
  if(!bePressed) {
    
    while(sendCmd(CMD_IS_FINGER_PRESSED, 0) > 0) {
      delay(200);  
    }  
  } else {
    while(sendCmd(CMD_IS_FINGER_PRESSED, 0) == 0) {
      delay(200);  
    }     
  }
}

void setup() {
  Serial.begin(115200);
  Serial3.begin(9600);
  Serial3.setTimeout(10*1000);
  delay(100);
  sendCmd(CMD_OPEN, 0);
  sendCmd(CMD_LED, 1);
  pinMode(resetButtonPin, INPUT);
  pinMode(addButtonPin, INPUT); 
  pinMode(lockPin, OUTPUT);
  pinMode(speakerPin, OUTPUT);
  digitalWrite(resetButtonPin, HIGH);
  digitalWrite(addButtonPin, HIGH);   
  for(int i = 0; i < 20; i++) {
    pulseServo(lockPin, closed);
  }  
}

void loop() {
  if (digitalRead(resetButtonPin) == LOW) {	// <1>
      if(sendCmd(CMD_DELETE_ALL, 0) >= 0) {
        peep(5,lowPeep);
      } else {
        peep(2,lowPeep); // Already empty
      }
  }
  if (digitalRead(addButtonPin) == LOW) {	// <2>
      enrollFinger();
  }  
  if(sendCmd(CMD_GET_ENROLL_COUNT, 0) == 0)	// <3>
  {
    delay(100);
    return;    
  }

  if(sendCmd(CMD_IS_FINGER_PRESSED, 0) == 0) {
      sendCmd(CMD_CAPTURE_FINGER, 1);
      int ret = sendCmd(CMD_IDENTIFY, 0);	
      if(ret >= 0 && ret < 200) {	// <4>
        if(state == 0) {
          peep(1,highPeep);
          for(int i = 0; i < 20; i++) {
            pulseServo(lockPin, opened);	// <5>
          }  
          state = 1;
          Serial.println("Open");
        } else {
          Serial.println("Close");
          peep(1,lowPeep);
          for(int i = 0; i < 20; i++) {
            pulseServo(lockPin, closed);
          }  
          state = 0;          
        }
      } else {
        peep(5,lowPeep);  
      }
      WaitForFinger(true);
  }
}
