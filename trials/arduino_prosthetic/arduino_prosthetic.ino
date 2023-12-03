#include <Servo.h>


Servo servoindex;          // Define index servo

void setup() { 
  servoindex.attach(3);  // Set index servo to digital pin 3
  
} 

void loop() {            // Loop through motion tests
//alltomiddle();        // Example: alltovirtual
//delay(4000);           // Wait 4000 milliseconds (4 seconds)
  alltorest();           // Uncomment to use this
  delay(4000);           // Uncomment to use this
  alltomax();            // Uncomment to use this
  delay(2000);           // Uncomment to use this
 
  
 
}
// Motion to set the servo into "virtual" 0 position: alltovirtual
void alltomiddle() {         
  servoindex.write(90);
}
// Motion to set the servo into "rest" position: alltorest
void alltorest() {         
  servoindex.write(0);
}



// Motion to set the servo into "max" position: alltomax
void alltomax() {
  servoindex.write(180);

 
}