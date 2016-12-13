#ifndef loopControl_h
#define loopControl_h

#include "arduino.h"  //for mircos, and delayMicroseconds
#include "math.h"

void loopControlCount();
bool loopControlCountDown();
bool loopControlCountDownRelease();
bool loopControlLogPeriod();
bool loopControlWritePeriod();
void loopControlDelay();

#endif
