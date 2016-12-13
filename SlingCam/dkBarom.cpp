#include "dkBarom.h"

const static int baromPin = 9;
Adafruit_BMP280 bme(baromPin); // hardware SPI

bool baromStartCheck(){
  return bme.begin();
}
float baromPressure(){
  return bme.readPressure();
}
/*float baromTemp(){
  return bme.readTemperature();
}*/

