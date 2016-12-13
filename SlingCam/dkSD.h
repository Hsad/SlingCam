#ifndef dkSD_h
#define dkSD_h

#include <SPI.h> 
#include <SD.h> 

bool SDStartCheck();
bool SDWriteCheck();
void SDOpen(String fileName);  //Not sure if this should be void or return an afirmative
void SDClose();

void SDLog(String txt);
void SDLog(int txt);
void SDLog(unsigned int txt);
void SDLog(float txt);
void SDLog(long txt);
void SDLog(unsigned long txt);

//testing ground
//void SDPrimaryTest();
//void SDSecondaryTest();

#endif
