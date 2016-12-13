#include "dkBeep.h"

const static int beepPin = 17;

void beepBoop(){
  tone(beepPin, 440, 100);
}

