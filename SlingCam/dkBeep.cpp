#include "dkBeep.h"

const static int beepPin = 17;

void beepBoop(){
  tone(beepPin, 440, 100);
}

void beepBloop(){
  tone(beepPin, 150, 100);
  delay(100);
}
