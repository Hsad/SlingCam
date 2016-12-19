#include "dkBeep.h"

const static int beepPin = 17;

void beepBoop(){
  tone(beepPin, 440, 100);
}

void beepBloop(){
  tone(beepPin, 150, 100);
  delay(100);
}

void beepTriple(){
  tone(beepPin, 650, 100);
  delay(150);
  tone(beepPin, 650, 100);
  delay(150);
  tone(beepPin, 650, 100);
  delay(150);
}

void beepPip(){
  tone(beepPin, 650, 30);
}

