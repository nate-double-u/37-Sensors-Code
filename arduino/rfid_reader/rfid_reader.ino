// rfid_reader.ino - read 125 kHz RFID tags with ELB149C5M electronic brick
// (c) BotBook.com - Karvinen, Karvinen, Valtokari
// Requires Arduino Mega for extra serial port


int bytesRead = 0;	// <1>
char buffer[13];	// <2>

void setup() {
  Serial.begin(115200);	// computer
  Serial3.begin(9600);	// RFID reader	// <3>
}

void loop() {
  char recv;
  if(Serial3.available() > 0) {	// <4>
    recv = Serial3.read();
    if(recv == 0x02) {	// <5>
      bytesRead = 0;
      Serial.println("Start reading tag");  
    } else if(bytesRead == 12 && recv == 0x03) {	// <6>
      Serial.println();  
      String data = buffer;
      byte checksum = 0;
      byte chk = toLong(data.substring(10, 12));
      long id = toLong(data.substring(4, 10));
      for(int i = 0; i < 10; i=i+2) {
        checksum ^= toLong(data.substring(i, i+2));	// <7>
      }
      Serial.print(id);	// <8>
      if(checksum == chk) {	// <9>
        Serial.println(" Card ok");
      } else {
        Serial.println(" Checksum error!");  
      }
    } else {
      buffer[bytesRead] = recv;
      bytesRead++;
      Serial.print(recv);
    }
  }
  delay(10);
}

long toLong(String data) {
  char buf[20];
  data = "0x"+data;
  data.toCharArray(buf, 19);
  return strtol(buf, NULL, 0);
  
}
