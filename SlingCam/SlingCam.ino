#include "dkServo.h"
#include "dkBarom.h"
#include "dkSD.h"
#include "dkBeep.h"

//void SDTertiaryTest();

bool startChecks[4] = {1,1,1,1};  //assume true until proven otherwise
void setup() {
  servoAttach();  //attach servo
  servoClose();  //set servo to proper location
  startChecks[0] = baromStartCheck();  //test barometer
  startChecks[1] = SDStartCheck();  //test SD card connection
  startChecks[2] = SDWriteCheck();  //test write to card
  //startChecks[3] = //test Camera

  //TODO thing that lists of the non funtioning components
  while( true ){ //want the progress to halt if any component is non functioning
    int count = 0;
    for (int s = 0; s < 4; s++){  //then beep for each failed component
      if( !startChecks[s] ){ 
        count++;
        for( int n = 0; n <= s; n++){  //beep count to indicate which failed
          beepBoop();
          delay(50); 
        }
        delay(200); //pause between each component
      }
    }
    delay(1000);  //pause between full cycles
    if (count == 0){  //everything worked fine, get out of this death trap
      break; 
    }
  }

  
  //SDPrimaryTest();
  //SDSecondaryTest();
  //SDTertiaryTest();
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

/*
void SDTertiaryTest(){
  SDOpen("TertTest.txt");
  SDLog("hello there");
  int x = 82;
  SDLog(x);
  SDClose();
}
*/



