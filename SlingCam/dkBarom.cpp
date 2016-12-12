#include "dkBarom.h"

int baromPin = 9;
Adafruit_BMP280 bme(baromPin); // hardware SPI

bool baromStartCheck(){
  return bme.begin();
}
float getPressure(){
  return bme.readPressure();
}
/*float getTemp(){
  return bme.readTemperature();
}*/

