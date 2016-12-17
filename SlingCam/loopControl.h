#ifndef loopControl_h
#define loopControl_h

#include "arduino.h"  //for mircos, and delayMicroseconds
#include "math.h"

//This is plopped in here to allow use of interval, which is dependant on loopTimeBlock
static unsigned long lastLoopTime = 0;  
static unsigned long lastLoopDiff;  //hold the difference after calculating it. Overflow protection
const static unsigned long loopTimeBlock = 1250; // 5000==5ms, can't be larger than 16ms or delayMicroseconds() will break

static int loopCount = 0;
const static float floatInterval = 1000.0 / (loopTimeBlock/1000.0) ;
const static int interval = round(floatInterval);

static int timeOverFlowCount = 0; //how manytimes there was no added delay because the calcualations blew past the regulation

void loopControlCount();
bool loopControlCountDown();
bool loopControlCountDownRelease();

bool loopControlLogPeriod0();
bool loopControlWritePeriod0();
int loopControlLogIndexHelper0();

bool loopControlLogPeriod1();
bool loopControlWritePeriod1();
int loopControlLogIndexHelper1();

bool loopControlLogPeriod2();
bool loopControlWritePeriod2();
int loopControlLogIndexHelper2();

void loopControlDelay();

int getLoopCount();
int getOverFlowCount();

#endif
