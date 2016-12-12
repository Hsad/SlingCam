#include <Servo.h>
#include "dkServo.h"

Servo myservo;
int servoPin = 4;
int servoCloseNumber = 90;
int servoOpenNumber = 145;

void servoAttach(){
  myservo.attach(servoPin);
}
void servoClose(){
  myservo.write(servoCloseNumber);  
}
void servoOpen(){
  myservo.write(servoOpenNumber);
}

