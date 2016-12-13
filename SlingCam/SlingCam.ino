#include "dkServo.h"
#include "dkBarom.h"
#include "dkSD.h"
#include "dkBeep.h"

bool startChecks[4] = {1,1,1,1};  //assume true until proven otherwise
void setup() {
  servoAttach();  //attach servo
  servoClose();  //set servo to proper location
  startChecks[0] = baromStartCheck();  //test barometer
  startChecks[1] = SDStartCheck();  //test SD card connection
  //startChecks[2] = SDWriteCheck();  //test write to card
  //startChecks[3] = //test Camera
  SDPrimaryTest();
  beepBoop();
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
