#include "dkRelease.h"
#include "dkSD.h"  //DEBUG


//apparently secret sauce is pretty quick to code up.
bool releaseNow(float pressure){
  if (releaseLoopCount == 0){ //initilize data
    ave = pressure;
    minAve = pressure;
    maxAve = pressure;
    releaseLoopCount++;
    return false;
  }

  if (pressure < ave){ //if low value, incorporate into the low average
    minAve = (pressure + (minAve * releaseLoopCount)) / (releaseLoopCount+1);
  }
  if (pressure > ave){ //if high value incorporate into the high average
    maxAve = (pressure + (maxAve * releaseLoopCount)) / (releaseLoopCount+1);
  }

  ave = (pressure + (ave * releaseLoopCount)) / (releaseLoopCount+1); //calculate baseline average
  releaseLoopCount++; //inc for next loop
  
  if(releaseLoopCount > 100){  //if not one of the early data points, which are error prone, and trigger happy.
    //if the pressure value is five times the min-max gap below the average, pop the chute
    difference = minAve - maxAve; //this is a negitive number
    if (pressure - ave < difference * 5){  //value zeroed out on the average.  p-a must be negitive
      //pressure had dropped more than 5 differences below average, pop the chute

      //DEBUG
      /*
      SDLog("Next 5, diff, ave, minAve, MaxAve, pressure, releaseLoopCount");
      SDLog(difference);
      SDLog(ave);
      SDLog(minAve);
      SDLog(maxAve);
      SDLog(pressure);
      SDLog(releaseLoopCount);
      */
      //DEBUG
      
      return true;
    }
  }
  return false;  //stay frosty
}

