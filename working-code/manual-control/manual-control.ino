#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

// Depending on your servo make, the pulse width min and max may vary, you
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN 250  // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 700  // this is the 'maximum' pulse length count (out of 4096)

// our servo # counter
uint8_t servonum = 0;
//int trigger= 2;
void setup() {
  Serial.begin(9600);
  Serial.println("5 channel Servo test!");
  //pinMode(trigger,INPUT);
  pwm.begin();

  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  delay(10);
}

void loop() {
  switch(Serial.read()){
    case 'O':openPalm();break;
    case 'C':closePalm();break;
  }
  }

void openPalm() {
  pwm.setPWM(0, 0, 110);
  delay(2);
  pwm.setPWM(1, 0, 125);
  delay(2);
  pwm.setPWM(3, 0, 125);
  delay(2);
  pwm.setPWM(4, 0, 125);
  delay(2);
  pwm.setPWM(6, 0, 580);
  delay(2);
}

void closePalm(){
  pwm.setPWM(0, 0, 650);
  delay(2);
  pwm.setPWM(1, 0, 590);
  delay(2);
  pwm.setPWM(3, 0, 640);
  delay(2);
  pwm.setPWM(4, 0, 550);
  delay(2);
  pwm.setPWM(6, 0, 100);
  delay(2);
}
