// fingerprint_scanner.ino - learn and recognize fingerprints with GT-511C3
// (c) BotBook.com - Karvinen, Karvinen, Valtokari
// Requires Arduino Mega for extra serial port




const byte STX1 = 0x55;	// <1>
const byte STX2 = 0xAA;

const word CMD_OPEN = 0x01;	// <2>
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

const word ACK = 0x30;	// <3>
const word NACK = 0x31;  //Error

struct package {	// <4>
  byte header1;
  byte header2;  
  word deviceID;
  unsigned long param;
  word cmd;
  word checksum;
};

const int SIZE_OF_PACKAGE = 12;

/*
To calculate checksum we add all bytes in pdu together.
*/
word calcChecksum(struct package *pkg) {	// <5>
  word checksum = 0;
  byte *buffer = (byte*)pkg;	// <6>
  for(int i=0; i < (sizeof(struct package) - sizeof(word)); i++) 
  {
    checksum += buffer[i];  
  }
  return checksum;
}


int sendCmd(word cmd, int param) {	// <7>
  struct package pkg;
  pkg.header1 = STX1;
  pkg.header2 = STX2;
  pkg.deviceID = 1;	// <8>
  pkg.param = param;
  pkg.cmd = cmd;
  pkg.checksum = calcChecksum(&pkg);
  //Serial.println("Sending command");
  byte *buffer = (byte*)&pkg;	// <9>

  int bytesSent = Serial3.write(buffer, sizeof(struct package));

  if(bytesSent != sizeof(struct package)) {
    Serial.println("Error communicating");
    return -1; 
  }

  int bytesReceived = 0;
  char recvBuffer[SIZE_OF_PACKAGE];	// <10>
  struct package *recvPkg = (struct package*) recvBuffer;	// <11>
 
  
  bytesReceived = Serial3.readBytes(recvBuffer, sizeof(struct package));	// <12>
  if(bytesReceived != SIZE_OF_PACKAGE) {
    Serial.println("Error communicating");
    return -1;
  }  

  if( recvPkg->header1 != STX1 || recvPkg->header2 != STX2) {	// <13>
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

//All custom codes here as they may use variables defined in protocol implementation.
void setup() {
  Serial.begin(115200);	// <14>
  Serial3.begin(9600);	// <15>
  Serial3.setTimeout(10*1000); // ms	// <16>
}

void flashLed(int time) {
  sendCmd(CMD_LED, 1);
  delay(time);
  sendCmd(CMD_LED, 0);
  
}

void loop() {
  Serial.println("Sending open command");
  sendCmd(CMD_OPEN, 0);
  //Delete all fingerprints on start for testing purpose only. 
  if(sendCmd(CMD_DELETE_ALL, 0) >= 0) {
    //Flash LED 3 times for victory dance and to indicate that we are ready for enrolling.
    flashLed(500);
    delay(500);  
    flashLed(500);
    delay(500);
    flashLed(500); 
  }

  Serial.println("Starting capture");
  
  int id = 0;
  id = sendCmd(CMD_GET_ENROLL_COUNT, 0);	// <17>
  sendCmd(CMD_LED, 1);
  sendCmd(CMD_ENROLL_START, id);
  Serial.println("Press finger to start enroll");
  int ret = 0;
  WaitForFinger(false);
  Serial.println("Capturing finger");
  ret = sendCmd(CMD_CAPTURE_FINGER, 1);	// <18>
  if(ret < 0) {
    EnrollFail();
    return;  
  }
  Serial.println("Remove finger");

  sendCmd(CMD_ENROLL_1, 0);
  WaitForFinger(true);
  Serial.println("Press finger again");

  WaitForFinger(false);
  ret = sendCmd(CMD_CAPTURE_FINGER, 1);
  if(ret < 0) {
    EnrollFail();
    return;  
  }
  Serial.println("Remove finger");

  sendCmd(CMD_ENROLL_2, 0);
  WaitForFinger(true);
  Serial.println("Press finger again");

  WaitForFinger(false);    
  ret = sendCmd(CMD_CAPTURE_FINGER, 1);
  if(ret < 0) {
    EnrollFail();
    return;  
  }
  Serial.println("Remove finger");

  ret = sendCmd(CMD_ENROLL_3, 0);
  if(ret != 0) {
    EnrollFail();
    return;  
  }  
  WaitForFinger(true);
  flashLed(500);
  delay(500);  
  flashLed(500);
  delay(500); 
  Serial.println("Enroll completed");
  Serial.println("Press finger for identify");
  sendCmd(CMD_LED, 1);  
  
  // Identify
  WaitForFinger(false);  
  ret = sendCmd(CMD_CAPTURE_FINGER, 1);	// <19>
  if(ret < 0) {
    IdentFail();
    return;  
  }  
  ret = sendCmd(CMD_IDENTIFY, 0);	// <20>
  if(ret >= 0 && ret < 200) {
    Serial.print("ID found: ");
    Serial.println(ret);
    flashLed(500);
    delay(500);  
    flashLed(500);
    delay(500);  
    flashLed(500);
    delay(500); 
    flashLed(500);
    delay(500);  
    flashLed(500);
    delay(500);  
    flashLed(500);
    delay(500);   
  } else {
    Serial.println("ID not found");
  }
  sendCmd(CMD_CLOSE,0);
  delay(100000);

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
// Flash LED 3 times for failure
// and close device.
void IdentFail() {
  Serial.println("Ident failed!");
  flashLed(500);
  delay(500);  
  flashLed(500);
  delay(500);  
  flashLed(500);
  delay(500);    
  sendCmd(CMD_CLOSE, 0);    
}
// Flash LED 4 times for failure
// and close device.
void EnrollFail() {
  Serial.println("Enroll failed!");
  flashLed(500);
  delay(500);  
  flashLed(500);
  delay(500);  
  flashLed(500);
  delay(500);
  flashLed(500);
  sendCmd(CMD_CLOSE, 0);    
}
