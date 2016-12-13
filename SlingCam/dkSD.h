#ifndef dkSD_h
#define dkSD_h

#include <SPI.h> 
#include <SD.h> 

bool SDStartCheck();
bool SDWriteCheck();
File SDOpen(String fileName);  //Not sure if this should be void or return an afirmative
void SDClose();

//testing ground
void SDPrimaryTest();
void SDSecondaryTest();

#endif
