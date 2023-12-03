#if defined(ESP32) 
  #include <ESP32Servo.h>
#else
  #include <Servo.h>
#endif
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SAMPLE_RATE 500
#define BAUD_RATE 115200
#define INPUT_PIN A0
#define BUFFER_SIZE 128
//#define SERVO_PIN 9
#define EMG_MIN 30
#define EMG_MAX 60
#define SERVOMIN 250  // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 600  // this is the 'maximum' pulse length count (out of 4096)

// our servo # counter
uint8_t servonum = 0;

int circular_buffer[BUFFER_SIZE];
int data_index, sum;

//Servo servo;

void setup() {
	// Serial connection begin
	Serial.begin(BAUD_RATE);
  Serial.println("5 channel Servo test!");
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  delay(10);
  // Attach servo
  //servo.attach(SERVO_PIN);
}

void loop() {
	// Calculate elapsed time
	static unsigned long past = 0;
	unsigned long present = micros();
	unsigned long interval = present - past;
	past = present;

	// Run timer
	static long timer = 0;
	timer -= interval;

	// Sample and get envelop
	if(timer < 0) {
		timer += 1000000 / SAMPLE_RATE;
		int sensor_value = analogRead(INPUT_PIN);
		int signal = EMGFilter(sensor_value);
		int envelop = getEnvelop(abs(signal));
    int servo_position[5];
    servo_position[0] = map(envelop, EMG_MIN, EMG_MAX, 550, 125);
    servo_position[1] = map(envelop, EMG_MIN, EMG_MAX, 125, 590);
    servo_position[2] = map(envelop, EMG_MIN, EMG_MAX, 125, 640);
    servo_position[3] = map(envelop, EMG_MIN, EMG_MAX, 125, 550);
    servo_position[4] = map(envelop, EMG_MIN, EMG_MAX, 145, 580);

    //servo.write(servo_position);
    Palm(servo_position[0],servo_position[1],servo_position[2],servo_position[3],servo_position[4]);
		for(int i=1;i<=5;i++){
    Serial.print(signal);
		Serial.print(",");
    Serial.println(servo_position[i-1]);

  }
	}
}

// Envelop detection algorithm
int getEnvelop(int abs_emg){
	sum -= circular_buffer[data_index];
	sum += abs_emg;
	circular_buffer[data_index] = abs_emg;
	data_index = (data_index + 1) % BUFFER_SIZE;
	return (sum/BUFFER_SIZE) * 2;
}


void Palm(int p,int r,int m,int i,int t) {
  pwm.setPWM(0, 0, p);
  delay(2);
  pwm.setPWM(1, 0, r);
  delay(2);
  pwm.setPWM(3, 0, m);
  delay(2);
  pwm.setPWM(4, 0, i);
  delay(2);
  pwm.setPWM(6, 0, t);
  delay(2);
}

// Band-Pass Butterworth IIR digital filter, generated using filter_gen.py.
// Sampling rate: 500.0 Hz, frequency: [74.5, 149.5] Hz.
// Filter is order 4, implemented as second-order sections (biquads).
// Reference: 
// https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.butter.html
// https://courses.ideate.cmu.edu/16-223/f2020/Arduino/FilterDemos/filter_gen.py
float EMGFilter(float input)
{
  float output = input;
  {
    static float z1, z2; // filter section state
    float x = output - 0.05159732*z1 - 0.36347401*z2;
    output = 0.01856301*x + 0.03712602*z1 + 0.01856301*z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - -0.53945795*z1 - 0.39764934*z2;
    output = 1.00000000*x + -2.00000000*z1 + 1.00000000*z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - 0.47319594*z1 - 0.70744137*z2;
    output = 1.00000000*x + 2.00000000*z1 + 1.00000000*z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - -1.00211112*z1 - 0.74520226*z2;
    output = 1.00000000*x + -2.00000000*z1 + 1.00000000*z2;
    z2 = z1;
    z1 = x;
  }
  return output;
}