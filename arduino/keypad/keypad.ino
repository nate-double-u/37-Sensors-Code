// keypad.ino - read 16-key numeric keypad (dx.com sku 149608)
// (c) BotBook.com - Karvinen, Karvinen, Valtokari

const int count = 4;	// <1>
char keymap[count][count] = {	// <2>
  {'1', '2', '3', 'A'},  
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
const char noKey = 'n';
byte columns[count] = {9, 8, 7, 6};	// <3>
byte rows[count] = {5, 4, 3, 2};
unsigned int lastReadTime;
unsigned int bounceTime = 30; // ms

void setup() 
{
  Serial.begin(115200);
  lastReadTime = millis();

  for(int i = 0; i < count; i++)
  {
    pinMode(rows[i], INPUT);
    digitalWrite(rows[i], HIGH); // pull up	// <4>

    pinMode(columns[i], INPUT);
  }
}

void loop()
{
  char key = getKeyPress();
  if(key != noKey) {
    Serial.print(key);
  }  
  delay(100);
}
// This does not support multiple presses. first one is 
// returned
char getKeyPress()
{
  char foundKey = noKey;
  if((millis() - lastReadTime) > bounceTime) {	// <5>
    //Pulse columns and read row pins
    for(int c = 0; c < count; c++) {
      //Start pulse
      pinMode(columns[c], OUTPUT);	// <6>
      digitalWrite(columns[c], LOW);
      //Read rows
      for(int r = 0; r < count; r++) {
        if(digitalRead(rows[r]) == LOW) {	// <7>
          //Find right character
          foundKey = keymap[r][c];	// <8>
        }  
      }
      digitalWrite(columns[c], HIGH);
      pinMode(columns[c], INPUT);	// <9>
      if(foundKey != noKey) 
      {
        break;
      }
    } 
    lastReadTime = millis();  
  }
 return foundKey; 
}
