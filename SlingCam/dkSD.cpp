#include "dkSD.h"

const static int SDPin = 10;
File dataFile;

bool SDStartCheck(){
  return SD.begin(SDPin);
}
bool SDWriteCheck(){
  //TODO
  //open file and check if it is open, then close it, return result
  SDOpen("WriteCK.txt");
  bool ret;
  if (dataFile){
    ret = true;
  } else{
    ret = false;
  }
  SDClose();
  return ret;
}
//Not sure if this should be void or return an afirmative
void SDOpen(String fileName){ 
  dataFile = SD.open(fileName, FILE_WRITE);
}
void SDClose(){
  dataFile.close();
}

//sort of want a print statement, but I'm not sure what I would need to do to deal with the
//multitide of incomming types
//Answer: Nothing, it's not C code any more!
void SDLog(String txt){
  dataFile.println(txt);
}
void SDLog(int txt){
  dataFile.println(txt);
}
void SDLog(unsigned int txt){
  dataFile.println(txt);
}
void SDLog(float txt){
  dataFile.println(txt);
}
void SDLog(long txt){
  dataFile.println(txt);
}
void SDLog(unsigned long txt){
  dataFile.println(txt);
}
//missing types: char*, short, unsigned short, long long, double, long double



/*
void SDPrimaryTest(){
  dataFile = SD.open("Primary.txt", FILE_WRITE);
  dataFile.println("Primary functions!");
  dataFile.close();
}

void SDSecondaryTest(){
  dataFile = SD.open("Secondar.txt", FILE_WRITE);
  String st = "String";
  int in = -10;
  unsigned int uni = 20;
  float flo = 0.1;
  long lo = -2147483600;
  unsigned long ulo = 2147500000;
  SDLog(st);
  SDLog(in);
  SDLog(uni);
  SDLog(flo);
  SDLog(lo);
  SDLog(ulo);
  SDLog("raw String");
  dataFile.close();
}
*/

