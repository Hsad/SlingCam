#include "dkServo.h"
#include "dkBarom.h"
#include "dkSD.h"
#include "dkBeep.h"
#include "loopControl.h"

//void SDTertiaryTest();

bool startChecks[4] = {1,1,1,1};  //assume true until proven otherwise

const int logArraySize = interval;
//float logArray[logArraySize];  //TODO  Need code that syncs up the number of loops within each interval
//Also TODO, figure out why having an array breaks everything...
String fileName = "test9.txt";

static unsigned long quickLoopStart;
static unsigned long quickLoopStart2; //Test7
bool Once = true;

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
          beepBloop();
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
  SDOpen(fileName);  //open the file, for when the array storage is non functioning
  beepTriple();
  delay(400);  
  beepBoop();
  
  quickLoopStart = micros();
}

void loop() {
  loopControlCount();  //increment the count
  if(loopControlCountDown()){  //if the countdown is at its beep interval
    beepBoop();
  }
  if(loopControlCountDownRelease()){  //on the fourth interval
    servoOpen();
    
  }


  //DATA RECORDING//
  //if(loopControlLogPeriod1()){ 
  if(loopControlLogPeriod0()){  
    //logArray[loopControlLogIndexHelper1()] = baromPressure();  //log pressure
    //that doesing seem to be working, so instead...
    SDLog(baromPressure());
    if(Once){  //Test7
      quickLoopStart2 = micros();
      Once = false;
    }
  }
  //if(loopControlWritePeriod1()){  //write the data from the log to the file
  if(loopControlWritePeriod0()){  //write the data from the log to the file
    //SDOpen(fileName);  //off because the file should already be open.
    SDLog("full time covered:");
    SDLog(micros() - quickLoopStart);
    SDLog("collection time covered:");  //during test9 this is basicly meaning less 
    SDLog(micros() - quickLoopStart2);
    SDLog("loop Count");
    SDLog(getLoopCount());  //was just loopCount, which apparently doesn't work.  I need to get more familiar with the finerpoints of including stuff
    SDLog("interval");
    SDLog(interval);
    SDLog("index helper");
    SDLog(loopControlLogIndexHelper1());
    SDLog("logArraySize");
    SDLog(logArraySize);    
    SDLog("times loop over limit");
    SDLog(getOverFlowCount());
    SDLog("loopTimeDelay:");
    SDLog(loopTimeBlock);
    //for(int x = 0; x < logArraySize; x++){
    //  SDLog(logArray[x]);
    //}
    SDClose();  //single close of the file.
    beepTriple();
  }

  //LOOP DELAY//
  loopControlDelay();  //delay to standardize time


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



