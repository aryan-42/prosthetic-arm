#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Vector.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

const byte buffSize = 40;
char inputBuffer[buffSize];
const char startMarker = '<';
const char endMarker = '>';
byte bytesRecvd = 0;
boolean readInProgress = false;
boolean newDataFromPC = false;
int finger[5];
char messageFromPC[buffSize] = {0};
unsigned long curMillis;

unsigned long prevReplyToPCmillis = 0;
unsigned long replyToPCinterval = 1000;

//=============

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  delay(10);
  Serial.println("<Arduino is ready>");
}

//=============

void loop() {
  getDataFromPC();
  if(finger[0]==1){pwm.setPWM(0, 0, 145);}else{pwm.setPWM(0, 0, 580);}
  if(finger[1]==1){pwm.setPWM(1, 0, 125);}else{pwm.setPWM(1, 0, 550);}
  if(finger[2]==1){pwm.setPWM(3, 0, 125);}else{pwm.setPWM(3, 0, 640);}
  if(finger[3]==1){pwm.setPWM(4, 0, 125);}else{pwm.setPWM(4, 0, 550);}
  if(finger[4]==1){pwm.setPWM(6, 0, 550);}else{pwm.setPWM(6, 0, 125);}
}

Vector<int> stringToIntArray(const String& str) {
    Vector<int> intArray;

    for (char ch : str) {
        int digit = ch - '0';
        intArray.push_back(digit);
    }
    return intArray;
}


//=============

void getDataFromPC() {

    // receive data from PC and save it into inputBuffer
    
  if(Serial.available() > 0) {

    char x = Serial.read();

      // the order of these IF clauses is significant
      
    if (x == endMarker) {
      readInProgress = false;
      newDataFromPC = true;
      inputBuffer[bytesRecvd] = 0;
      parseData();
    }
    
    if(readInProgress) {
      inputBuffer[bytesRecvd] = x;
      bytesRecvd ++;
      if (bytesRecvd == buffSize) {
        bytesRecvd = buffSize - 1;
      }
    }

    if (x == startMarker) { 
      bytesRecvd = 0; 
      readInProgress = true;
    }
  }
}

//=============
 
void parseData() {
finger={0,0,0,0,1}; //= stringToIntArray(inputBuffer);
}

void replyToPC() {
  if (newDataFromPC) {
    newDataFromPC = false;
    Serial.print("Converted array: [");
    for (size_t i = 0; i < 5; ++i) {
        Serial.print(finger[i]);
        if (i < 5 - 1) {
            Serial.print(", ");
        }
    }
    Serial.println("]");

  }
}






