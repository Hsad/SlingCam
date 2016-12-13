#include "dkServo.h"
#include "dkBarom.h"
#include "dkSD.h"
#include "dkBeep.h"
#include "loopControl.h"

//void SDTertiaryTest();

bool startChecks[4] = {1,1,1,1};  //assume true until proven otherwise

float logArray[10];
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
  loopControlCount();  //increment the count
  if(loopControlCountDown()){  //if the countdown is at its beep interval
    beepBoop();
  }
  if(loopControlCountDownRelease()){  //on the fourth interval
    servoOpen();
  }

  if(loopControlLogPeriod()){  //log data between third and fourth interval
    //need accurate intervals 
    //then need a system that ideally coordinates the speed of the loop with 
    //the given size of the array so that the interval covered
    //is equivelent to the number of cycles in the period
  }
  if(loopControlWritePeriod()){  //write the data from the log to the file
    //make sure it doesn't delay release
  }

  //delay to standardize time
  loopControlDelay();


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



