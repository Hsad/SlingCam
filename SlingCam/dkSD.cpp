#include "dkSD.h"

const static int SDPin = 10;
File dataFile;

bool SDStartCheck(){
  return SD.begin(SDPin);
}
bool SDWriteCheck(){
  //TODO
  //open file and check if it is open, then close it, return result
  
}
//Not sure if this should be void or return an afirmative
File SDOpen(String fileName){  //now I dont know weather to return a reference or a pointer or what...
  dataFile = SD.open(fileName, FILE_WRITE);
  return dataFile;
}
//sort of want a print statement, but I'm not sure what I would need to do to deal with the
//multitide of incomming types
void SDClose(){
  dataFile.close();
}

void SDPrimaryTest(){
  dataFile = SD.open("Primary.txt", FILE_WRITE);
  dataFile.println("Primary functions!");
  dataFile.close();
}

