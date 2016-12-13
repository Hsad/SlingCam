#include "dkServo.h"

Servo myservo;
const static int servoPin = 4;
const static int servoCloseNumber = 90;
const static int servoOpenNumber = 145;

void servoAttach(){
  myservo.attach(servoPin);
}
void servoClose(){
  myservo.write(servoCloseNumber);  
}
void servoOpen(){
  myservo.write(servoOpenNumber);
}

