#include "dkServo.h"
#include "dkBarom.h"

bool startChecks[4] = {0,0,0,0};
void setup() {
  servoAttach();  //attach servo
  servoClose();  //set servo to proper location
  startChecks[0] = baromStartCheck();  //test barometer
  startChecks[1] = SDStartCheck();  //test SD card connection
  startChecks[2] = SDWriteCheck();  //test write to card
  //startChecks[3] = //test Camera
  
}

void loop() {
  //beep on every interval
  //on fouth interval pop chute
  //log data between third and fourth interval
  //write data after fouth interval. 
    //make sure it doesn't delay release
  //delay to standardize time



  //FINAL ROUTINE//
  //check altitude
    //if acceleration upward is slowing
  //Pop Chute
  //take photo
}
