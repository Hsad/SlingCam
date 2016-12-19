#ifndef dkCam_h
#define dkCam_h

#include "dkBeep.h"
#include "dkSD.h"


#include <Adafruit_VC0706.h>

//might need the SPI include
#include <SPI.h> 
#include <SD.h>

#include <SoftwareSerial.h>
//#include <HardwareSerial.h>


//I still dont know what part of this is actually being used
/////////////
// Using SoftwareSerial (Arduino 1.0+) or NewSoftSerial (Arduino 0023 & prior):
#if ARDUINO >= 100
// On Uno: camera TX connected to pin 2, camera RX to pin 3:
SoftwareSerial cameraconnection = SoftwareSerial(1, 0);  ///////////////////// OK SO ONE OF THESE MUST WORK JUST GONNA TRY BOTH AND FIGURE IT OUT...
// On Mega: camera TX connected to pin 69 (A15), camera RX to pin 3:
//SoftwareSerial cameraconnection = SoftwareSerial(69, 3);
#else
NewSoftSerial cameraconnection = NewSoftSerial(1, 0);  /////////////////////////BLARG
#endif

//my attempt at this junk
//HardwareSerial dkCamConnect = HardwareSerial(1,0);
//Adafruit_VC0706 camera = Adafruit_VC0706(&dkCamConnect);
Adafruit_VC0706 camera = Adafruit_VC0706(&cameraconnection);
///////////

static char camFileName[13]; 

bool camStartCheck();
bool camPrepareFileName();
void camTakeAndSave();


#endif
