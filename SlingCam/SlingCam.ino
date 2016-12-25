#include "dkServo.h"
#include "dkBarom.h"
#include "dkSD.h"
#include "dkBeep.h"
#include "loopControl.h"
#include "dkRelease.h"
//#include "dkCam.h"



////////////////////////////////////
/////////////////////////////
/////////////////////////////
////////////////////////////////////


//My Attempt to force the cam to work...
#include <Adafruit_VC0706.h>

//might need the SPI include
#include <SPI.h> 
#include <SD.h>

#include <SoftwareSerial.h>

//I still dont know what part of this is actually being used
/////////////
// Using SoftwareSerial (Arduino 1.0+) or NewSoftSerial (Arduino 0023 & prior):
#if ARDUINO >= 100
// On Uno: camera TX connected to pin 2, camera RX to pin 3:
SoftwareSerial cameraconnection = SoftwareSerial(15, 14);  ///////////////////// OK SO ONE OF THESE MUST WORK JUST GONNA TRY BOTH AND FIGURE IT OUT...
// On Mega: camera TX connected to pin 69 (A15), camera RX to pin 3:
//SoftwareSerial cameraconnection = SoftwareSerial(69, 3);
#else
NewSoftSerial cameraconnection = NewSoftSerial(15, 14);  /////////////////////////BLARG
#endif

//my attempt at this junk
//HardwareSerial dkCamConnect = HardwareSerial(1,0);
//Adafruit_VC0706 camera = Adafruit_VC0706(&dkCamConnect);
Adafruit_VC0706 camera = Adafruit_VC0706(&cameraconnection);
///////////

static char camFileName[13]; 

//bool camStartCheck();
//bool camPrepareFileName();
//void camTakeAndSave();


///////////////////////////////////
///////////////////////////////
///////////////////////////////
///////////////////////////////////



//void SDTertiaryTest();

bool startChecks[5] = {1,1,1,1,1};  //assume true until proven otherwise

const int logArraySize = interval;
//float logArray[logArraySize];  //TODO  Need code that syncs up the number of loops within each interval
//Also TODO, figure out why having an array breaks everything...
char* fileName = "photo1.txt";

static unsigned long quickLoopStart;
static unsigned long quickLoopStart2; //Test7
bool Once = true;

bool chuteOpened = false;
bool photoNotTaken = true;

void setup() {
  SDClose(); //Might be getting an error where this continues to keep an old file open, blocking new files.
  servoAttach();  //attach servo
  servoClose();  //set servo to proper location
  startChecks[0] = baromStartCheck();  //test barometer
  startChecks[1] = SDStartCheck();  //test SD card connection
  startChecks[2] = SDWriteCheck();  //test write to card
  startChecks[3] = camStartCheck();  //test Camera
  startChecks[4] = camPrepareFileName();

  //TODO thing that lists of the non funtioning components
  while( true ){ //want the progress to halt if any component is non functioning
    int count = 0;
    for (int s = 0; s < 5; s++){  //then beep for each failed component
      if( !startChecks[s] ){ 
        count++;
        for( int n = 0; n <= s; n++){  //beep count to indicate which failed
          //beepBloop();
          beepBoop();
          delay(150); 
        }
        delay(200); //pause between each component
      }
    }
    delay(1000);  //pause between full cycles

    
    //check again... see if its fixed.  Now only the busted ones.
    if(!startChecks[0]){
      startChecks[0] = baromStartCheck();  //test barometer
    }
    if(!startChecks[1]){
      startChecks[1] = SDStartCheck();  //test SD card connection
    }
    if(!startChecks[2]){
      startChecks[2] = SDWriteCheck();  //test write to card
    }
    if(!startChecks[3]){
      startChecks[3] = camStartCheck();  //test Camera
    }

    
    if (count == 0){  //everything worked fine, get out of this death trap
      break; 
    }
  }

  
  //SDPrimaryTest();
  //SDSecondaryTest();
  //SDTertiaryTest();
  SDOpen(fileName);  //open the file, for when the array storage is non functioning
  beepTriple();
  delay(400);  
  beepBoop();
  
  quickLoopStart = micros();
}

void loop() {
  loopControlCount();  //increment the count
  if(loopControlCountDown()){  //if the countdown is at its beep interval
    beepBoop();
  }
  if(loopControlCountDownRelease()){  //on the fourth interval
    servoOpen();
    SDLog("Deploy");
    chuteOpened = true;
  }
  //Barom Controlled release
  if(releaseNow(baromPressure())){
    servoOpen();
    SDLog("Barom Deploy");
    chuteOpened = true;
  }

  if(chuteOpened and photoNotTaken){
    SDClose();  //close so that the cam can open a file
    delay(700); //delay a bit longer after release to maybe garner some extra stability during the photo
    servoDetach();
    camTakeAndSave();
    SDOpen(fileName);  
    photoNotTaken = false;
  }

  //DATA RECORDING//
  //if(loopControlLogPeriod1()){ 
  if(loopControlLogPeriod0()){  
    //logArray[loopControlLogIndexHelper1()] = baromPressure();  //log pressure
    //that doesing seem to be working, so instead...
    if(SDIsOpen()){
      SDLog(baromPressure());
    }
    if(Once){  //Test7
      quickLoopStart2 = micros();
      Once = false;
    }
  }
  //if(loopControlWritePeriod1()){  //write the data from the log to the file
  if(loopControlWritePeriod0()){  //write the data from the log to the file
    //SDOpen(fileName);  //off because the file should already be open.
    if(SDIsOpen()){
      //SDLog("full time covered:");
      //SDLog(micros() - quickLoopStart);
      //SDLog("collection time covered:");  //during test9 this is basicly meaning less 
      //SDLog(micros() - quickLoopStart2);
      //SDLog("loop Count");
      //SDLog(getLoopCount());  //was just loopCount, which apparently doesn't work.  I need to get more familiar with the finerpoints of including stuff
      //SDLog("interval");
      //SDLog(interval);
      //SDLog("index helper");
      //SDLog(loopControlLogIndexHelper1());
      //SDLog("logArraySize");
      //SDLog(logArraySize);    
      SDLog("times loop over limit");
      SDLog(getOverFlowCount());
      SDLog("loopTimeDelay:");
      SDLog(loopTimeBlock);
      //for(int x = 0; x < logArraySize; x++){
      //  SDLog(logArray[x]);
      //}
    }
    SDClose();  //single close of the file.
    beepTriple();
  }

  //LOOP DELAY//
  loopControlDelay();  //delay to standardize time


  //FINAL ROUTINE//
  //check altitude
    //if acceleration upward is slowing
  //Pop Chute
  //take photo

}

//bool deployAlgo(){
  //count data points, loopCount really
  //watch the average
  //watch the min average, values below the average
  //watch the max average, values above the average
  //calculate the difference, maxave - minave
  //detect if the value is some times less(presssure drops) than difference + the average
//}

/*
void SDTertiaryTest(){
  SDOpen("TertTest.txt");
  SDLog("hello there");
  int x = 82;
  SDLog(x);
  SDClose();
}
*/



bool camStartCheck(){
  if(camera.begin()){
    camera.setImageSize(VC0706_640x480);
    return true;
  }
  return false;
}

bool camPrepareFileName(){
  strcpy(camFileName, "IMAGE00.JPG");
  for (int i = 0; i < 100; i++) {
    camFileName[5] = '0' + i/10;
    camFileName[6] = '0' + i%10;
    // create if does not exist, do not open existing, write, sync after write
    if (!SDFileExists(camFileName)) {
      return true;
    }
  }
  return false;  //ran out of fileNames....
}


void camTakeAndSave(){
  if(!camera.takePicture()) {
    //flash(1,green);
    //Uhhh not sure If I can just have takePicture()
    return 0;
  }
  
  SDOpen(camFileName);
  uint16_t jpglen = camera.frameLength(); // Get the size of the image (frame) taken  

  byte wCount = 0; // For counting # of writes
  while (jpglen > 0) {
    // read 32 bytes at a time;
    uint8_t *buffer;
    uint8_t bytesToRead = min(32, jpglen); // change 32 to 64 for a speedup but may not work with all setups!
    buffer = camera.readPicture(bytesToRead);
    //imgFile.write(buffer, bytesToRead);
    SDCamWrite(buffer, bytesToRead);
    if(++wCount >= 64) { // Every 2K, give a little feedback so it doesn't appear locked up
      beepPip();
      wCount = 0;
    }
    //Serial.print("Read ");  Serial.print(bytesToRead, DEC); Serial.println(" bytes");
    jpglen -= bytesToRead;
  }
  SDClose();
  //imgFile.close();
}


