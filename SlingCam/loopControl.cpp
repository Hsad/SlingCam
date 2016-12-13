#include "loopControl.h"





    //need accurate intervals 
    //then need a system that ideally coordinates the speed of the loop with 
    //the given size of the array so that the interval covered
    //is equivelent to the number of cycles in the period
    //interval size
    //loop timeframe
    //interval time
    //ITms = IS# * LTms
    //arr = IS# * 2 //Division of odd numbers leads to a round down

    /* I need an independant variable that the rest of this changes off of.  
     *  I'm thinking the length of time that each loop comprised is a good one.
     *  also the interval size would somewhat make sense, although it is really steming off of an
     *  unlisted desired true time delay and the time comprised by each loop.
     *  looks like the old true interval was 1second or 1000ms
     *  so interval = 1000 / (loopTimeBlock/1000) = 1000 / 5 = 200
     *  and logArraySize = interval or interval*2 depending on the period used
     *  
     */

void loopControlCount(){
  loopCount++;
}
bool loopControlCountDown(){
  if(loopCount % interval == 0 and loopCount < interval*3){ //3 becasue its going to beep at zero
    return true;
  }
  return false;
}
bool loopControlCountDownRelease(){
  if(loopCount == interval * 3){
    return true;
  }
  return false;
}



//this is recording from the third beep to the parachute release
bool loopControlLogPeriod1(){  //log data between third and fourth interval
  if(loopCount >= interval * 2 and loopCount < interval*3){
    return true;
  }
  return false;
}
bool loopControlWritePeriod1(){ //make sure it doesn't delay release
  if(loopCount == interval * 3){
    return true;
  }
  return false;
}
int loopControlLogIndexHelper1(){
  return loopCount - interval*2;
}

//Double sized offset 
bool loopControlLogPeriod2(){  
  if(loopCount >= interval+(interval/2) and loopCount < interval*3+(interval/2)){
    return true;
  }
  return false;
}
bool loopControlWritePeriod2(){ //make sure it doesn't delay release
  if(loopCount == interval * 3 + (interval/2)){
    return true;
  }
  return false;
}
int loopControlLogIndexHelper2(){
  return loopCount - (interval + interval/2);
}


void loopControlDelay(){
  lastLoopDiff = micros()-lastLoopTime;  //becasue there is a really tiny chance this could underflow in the time that elapses, causing a massive delay, like 70 hours
  if(loopTimeBlock > lastLoopDiff){
    //m-t = time elapsed this loop, subract from ideal loop time
    delayMicroseconds(loopTimeBlock - lastLoopDiff); //can't handle delays longer than 16383, or 16ms
  }
  else{
    timeOverFlowCount++;
  }
  lastLoopTime = micros(); //micros for absurd accuracy, overflows in 70 minutes
}

