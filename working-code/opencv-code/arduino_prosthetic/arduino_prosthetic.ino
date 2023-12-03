#include <Servo.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

#define numOfValsRec 5
#define digitsPerValRec 1

uint8_t servonum = 0;


int valsRec[numOfValsRec];
int stringLength = numOfValsRec * digitsPerValRec + 1;  //$00000
int counter = 0;
//int buzzer=13;
bool counterStart = false;
String receivedString;

void setup() {
  Serial.begin(9600);
  Serial.println("5 channel Servo test!");

  pwm.begin();

  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  delay(10);
  //pinMode(buzzer,OUTPUT);
}

void receiveData() {
  while (Serial.available()) {
    char c = Serial.read();
    Serial.print(c);
    if (c =='$') {
      counterStart = true;
    }
    if (counterStart) {
      if (counter < stringLength) {
        receivedString = String(receivedString + c);
        Serial.print(receivedString);
        counter++;
      }
      if (counter >= stringLength) {
        for (int i = 0; i < numOfValsRec; i++) {
          int num = (i * digitsPerValRec) + 1;
          valsRec[i] = receivedString.substring(num, num + digitsPerValRec).toInt();
        }
        receivedString = "";
        counter = 0;
        counterStart = false;
      }
    }
  }
}

void loop() {
  receiveData();
  if(valsRec[0]==1){pwm.setPWM(0, 0, 550);}else{pwm.setPWM(0, 0, 125);}
  if(valsRec[1]==1){pwm.setPWM(1, 0, 125);}else{pwm.setPWM(1, 0, 590);}
  if(valsRec[2]==1){pwm.setPWM(3, 0, 125);}else{pwm.setPWM(3, 0, 640);}
  if(valsRec[3]==1){pwm.setPWM(4, 0, 125);}else{pwm.setPWM(4, 0, 550);}
  if(valsRec[4]==1){pwm.setPWM(6, 0, 125);}else{pwm.setPWM(6, 0, 580);}
}
