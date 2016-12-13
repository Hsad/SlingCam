#ifndef dkBarom_h
#define dkBarom_h

#include <SPI.h>

#include <Wire.h> //Barom
#include <Adafruit_Sensor.h> //Barom
#include <Adafruit_BMP280.h> //Barom

bool baromStartCheck();
float baromPressure();
//floar baromTemp();


#endif 
