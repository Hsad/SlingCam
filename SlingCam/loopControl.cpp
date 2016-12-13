#include "loopControl.h"

static int loopCount = 0;
const static int interval = 100;
static unsigned long lastLoopTime = 0;  
static unsigned long lastLoopDiff;  //hold the difference after calculating it. Overflow protection
const static unsigned long loopTimeBlock = 5000; // 5000==5ms, can't be larger than 16ms

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
bool loopControlLogPeriod(){  ///TODO  Set this interval counter to a proper system
  if(loopCount > interval * 1.5 and loopCount < interval*3){
    //at the moment this is recording from the third beep to the parachute release, my be better to have recoring from 
    //inbetween beep two and three, and an interval after the chute release
    return true;
  }
  return false;
}
bool loopControlWritePeriod(){
  if(loopCount == interval * 4){
    return true;
  }
  return false;
}
void loopControlDelay(){
  lastLoopDiff = micros()-lastLoopTime;  //becasue there is a really tiny chance this could underflow in the time that elapses, causing a massive delay, like 70 hours
  if(loopTimeBlock > lastLoopDiff){
    //m-t = time elapsed this loop, subract from ideal loop time
    delayMicroseconds(loopTimeBlock - lastLoopDiff); //can't handle delays longer than 16383, or 16ms
  }
  lastLoopTime = micros(); //micros for absurd accuracy, overflows in 70 minutes
}

