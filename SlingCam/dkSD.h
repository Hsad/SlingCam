#ifndef dkSD_h
#define dkSD_h

#include <SPI.h> 
#include <SD.h> 

bool SDStartCheck();
bool SDWriteCheck();
void SDOpen(char* fileName);  //Not sure if this should be void or return an afirmative
void SDClose();
bool SDIsOpen();
bool SDFileExists(char* fileName);


void SDLog(char* txt);
void SDLog(int txt);
void SDLog(unsigned int txt);
void SDLog(float txt);
void SDLog(long txt);
void SDLog(unsigned long txt);

void SDCamWrite(uint8_t *buffer, uint8_t bytesToRead);

//testing ground
//void SDPrimaryTest();
//void SDSecondaryTest();

#endif
