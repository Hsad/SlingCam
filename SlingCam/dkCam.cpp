#include "dkCam.h"


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
  //File imgFile = SDOpen(camFileName);
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

















