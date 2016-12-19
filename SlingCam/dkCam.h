#ifndef dkCam_h
#define dkCam_h

#include "dkBeep.h"
#include "dkSD.h"

//might need the SPI include
#include <SPI.h> 

#include <Adafruit_VC0706.h>
#include <SoftwareSerial.h>


//I still dont know what part of this is actually being used
/////////////
// Using SoftwareSerial (Arduino 1.0+) or NewSoftSerial (Arduino 0023 & prior):
#if ARDUINO >= 100
// On Uno: camera TX connected to pin 2, camera RX to pin 3:
SoftwareSerial cameraconnection = SoftwareSerial(1, 0);
// On Mega: camera TX connected to pin 69 (A15), camera RX to pin 3:
//SoftwareSerial cameraconnection = SoftwareSerial(69, 3);
#else
NewSoftSerial cameraconnection = NewSoftSerial(1, 0);
#endif
Adafruit_VC0706 camera = Adafruit_VC0706(&cameraconnection);
///////////

static char camFileName[13]; 

bool camStartCheck();
bool camPrepareFileName();
void camTakeAndSave();


#endif
