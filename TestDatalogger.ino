/*
  SD card datalogger

 This example shows how to log data from three analog sensors
 to an SD card using the SD library.

 The circuit:
 * analog sensors on analog ins 0, 1, and 2
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4

 created  24 Nov 2010
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */

#include <SPI.h>  //all
#include <SD.h>  //all


//Fruits stuff
#include <Adafruit_VC0706.h> //Cam
#include <SoftwareSerial.h> //Cam

#include <Wire.h> //Barom
#include <Adafruit_Sensor.h> //Barom
#include <Adafruit_BMP280.h> //Barom

#include <Servo.h>

#define BMP_SCK 13 //Barom
#define BMP_MISO 12 //Barom
#define BMP_MOSI 11  //Barom
#define BMP_CS 9 //Barom

//Adafruit_BMP280 bme; // I2C
Adafruit_BMP280 bme(BMP_CS); // hardware SPI
//Adafruit_BMP280 bme(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);
/////////////
// Using SoftwareSerial (Arduino 1.0+) or NewSoftSerial (Arduino 0023 & prior):
#if ARDUINO >= 100
// On Uno: camera TX connected to pin 2, camera RX to pin 3:
SoftwareSerial cameraconnection = SoftwareSerial(1, 0);
// On Mega: camera TX connected to pin 69 (A15), camera RX to pin 3:
//SoftwareSerial cameraconnection = SoftwareSerial(69, 3);
#else
NewSoftSerial cameraconnection = NewSoftSerial(1, 0);
#endif
Adafruit_VC0706 cam = Adafruit_VC0706(&cameraconnection);
///////////


const int chipSelect = 10;
const int bmeSelect = 9;
int blue = 3;
int green = 8;
int red = 6;
Servo myservo;
int servopin = 4;
int pos = 90; //servo

//Tone
int boop = 17;

//Direction Data
float PreviousAlt = 0;
float CurrentAlt = 0;
//float []  //Array of last alt

void setup() {
  //tripleFlash();
  // Open serial communications and wait for port to open:
  //Serial.begin(9600);
  //while (!Serial) {
  //  ; // wait for serial port to connect. Needed for native USB port only
  //}

  myservo.attach(servopin);

  // When using hardware SPI, the SS pin MUST be set to an
  // output (even if not connected or used).  If left as a
  // floating input w/SPI on, this can cause lockuppage.
  #if !defined(SOFTWARE_SPI)
  #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    if(chipSelect != 53) pinMode(53, OUTPUT); // SS on Mega
  #else
    if(chipSelect != 10) pinMode(10, OUTPUT); // SS on Uno, etc.
  #endif
  #endif

  ///WOOOOOOP///
  woopup();
  //delay(300);
  //woopdown();

  //HouseOfTheRisingSun();

  pinMode(blue, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
  //Serial.print("Initializing SD card...");
  delay(1000);
  tone(boop, 660, 100);
  delay(150);
  tone(boop, 360, 100);
  delay(150);
  tone(boop, 660, 100);
  // see if the card is present and can be initialized:
  while (!SD.begin(chipSelect)){
    //flash(2, blue);
    //beep(200);
    tone(boop, 260, 100);
    //tone(boop, 260, 100);
    delay(500);
  }
  delay(1000);
  tone(boop, 660, 100);
  delay(150);
  tone(boop, 360, 100);
  delay(150);
  tone(boop, 660, 100);
  //Wait for the barom to start
  while (!bme.begin()){
    //flash(2,green);
    //beep(300);
    tone(boop, 260, 100);
    delay(150);
    tone(boop, 260, 100);
    delay(500);
  }
  delay(1000);
  tone(boop, 660, 100);
  delay(150);
  tone(boop, 360, 100);
  delay(150);
  tone(boop, 660, 100);

  woopdown();
  File dataFile = SD.open("datalog2.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println("StartOfThrow");
    //dataFile.println(bme.readPressure());
    //dataFile.println(bme.readTemperature());
    dataFile.close();
  }

  
  //need to beable to detect upward motion.
  //need to detect a reversal in direction.
  //need to assure myself that this is an accurate detection.

  ///None of this will Run Until the Camera is mounted///

  /*
  //get the camera
  while (!cam.begin()){
    //flash(2,red);
    //beep(400);
    tone(boop, 260, 100);
    delay(150);
    tone(boop, 260, 100);
    delay(150);
    tone(boop, 260, 100);
    delay(1000);
  }
  cam.setImageSize(VC0706_640x480);        // biggest
  //cam.setImageSize(VC0706_320x240);        // medium
  //cam.setImageSize(VC0706_160x120);          // small
  
  
  //Take a picture, light up red and blue while it happens
  analogWrite(red,50); //take photo red led
  beep(500);
  while(!cam.takePicture()) {
    flash(1,green);
  }
  // Create an image with the name IMAGExx.JPG
  char filename[13];
  strcpy(filename, "IMAGE00.JPG");
  for (int i = 0; i < 100; i++) {
    filename[5] = '0' + i/10;
    filename[6] = '0' + i%10;
    // create if does not exist, do not open existing, write, sync after write
    if (! SD.exists(filename)) {
      break;
    }
  }

  // Open the file for writing
  File imgFile = SD.open(filename, FILE_WRITE);

  // Get the size of the image (frame) taken  
  uint16_t jpglen = cam.frameLength();

  analogWrite(blue,50); //save photo blue led
  beep(600);
  //Save photo
  pinMode(8, OUTPUT);  //<-- THE FUCK IS THIS DOING???
  // Read all the data up to # bytes!
  byte wCount = 0; // For counting # of writes
  while (jpglen > 0) {
    // read 32 bytes at a time;
    uint8_t *buffer;
    uint8_t bytesToRead = min(32, jpglen); // change 32 to 64 for a speedup but may not work with all setups!
    buffer = cam.readPicture(bytesToRead);
    imgFile.write(buffer, bytesToRead);
    if(++wCount >= 64) { // Every 2K, give a little feedback so it doesn't appear locked up
      flash(1,green);
      wCount = 0;
    }
    //Serial.print("Read ");  Serial.print(bytesToRead, DEC); Serial.println(" bytes");
    jpglen -= bytesToRead;
  }
  imgFile.close();
  beep(700);
  analogWrite(red,0);
  analogWrite(blue,0);
  
  flash(1, red);
  flash(1, blue);
  flash(1, green);

  
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(30);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(30);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 0; pos >= 90; pos += 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(30);                       // waits 15ms for the servo to reach the positiony
    
  }
  flash(2, red);
  flash(2, blue);
  flash(2, green);
  tone(boop, 660, 100);
  */
  myservo.write(90);
  delay(1000);
  //for (int tur = 90; tur > 54; tur--){
  //  myservo.write(tur);
  //  delay(500);
  //}
  myservo.write(115);
  delay(1000);
  //delay(10000);
  myservo.write(90);
  delay(1000);
  
}

void loop() {
  // make a string for assembling the data to log:
  //String dataString = "Hello Dash";


  
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog2.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    //dataFile.println(dataString);
    dataFile.println(bme.readPressure());
    //dataFile.println(bme.readTemperature());
    dataFile.close();
    // print to the serial port too:
    //Serial.println(dataString);
    //fullBright();
    //flash(1,red);
  }
  // if the file isn't open, pop up an error:
  else {
    //Serial.println("error opening datalog.txt");
    //halfBright();
    //flash(1,green);
    //flash(1,blue);
    beep(350);    
  }
  
  //SERVO SHIT
  /*
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  */
}



/*  LED Control
void fullBright(){
  analogWrite(3,50);
  delay(500);
  analogWrite(3,0);
  delay(10);
}

void flash(int numTimes, int pin, int delayTime){
  for (int x = 0; x < numTimes; x++){
    analogWrite(pin,50);
    delay(delayTime);
    analogWrite(pin,0);
    delay(delayTime);
  }
}
void flash(int numTimes, int pin){
  for (int x = 0; x < numTimes; x++){
    analogWrite(pin,50);
    delay(200);
    analogWrite(pin,0);
    delay(200);
  }
}
*/

void beep(int freq){
  tone(boop, freq, 100);
}

void woopup(){
  tone(boop, 220,100);
  delay(150);
  tone(boop, 246,100);
  delay(150);
  tone(boop, 260,100);
  delay(150);
  tone(boop, 290,100);
  delay(150);
  tone(boop, 330,100);
  delay(150);
  tone(boop, 350,100);
  delay(150);
  tone(boop, 390,100);
  delay(150);
  tone(boop, 440,100);
}

void woopdown(){
  tone(boop, 440,100);
  delay(150);
  tone(boop, 390,100);
  delay(150);
  tone(boop, 350,100);
  delay(150);
  tone(boop, 330,100);
  delay(150);
  tone(boop, 290,100);
  delay(150);
  tone(boop, 260,100);
  delay(150);
  tone(boop, 246,100);
  delay(150);
  tone(boop, 220,100);
}

void HouseOfTheRisingSun(){
  _A1();
  delay(400);
  _A1();
  delay(400);
  _B1();
  delay(400);
  _C1();
  delay(400);
  _D1();
  delay(200);
  _E1();
  delay(400);
  _D1();
  delay(400);
  _A1();
  delay(150);
  _A1();
  delay(400);
  _A1();
  delay(400);
  _A2();
  delay(400);
  _A2();
  delay(400);
  _A2();
  delay(400);
  _G1();
  delay(400);
  _E1();
  delay(400);
  _E1();
  delay(400);
  _A2();
  delay(400);
  _A2();
  delay(400);
  _A2();
  delay(400);
  _A2();
  delay(400);
  _G1();
  delay(400);
  _E1();
  delay(400);
  _D1();
  delay(400);
  _A1();
  delay(150);
  _A1();
  delay(400);
  _C1();
  delay(400);
  _A1();
  delay(400);
  _A1();
  delay(400);
  _A1();
  delay(400);
  _A1();
  delay(400);
  _A1Flat();
  delay(400);
  _B1();
  delay(400);
  _A1();
  delay(400);
}

void _A1Flat(){
  tone(boop, 207,100);
}
void _A1(){
  tone(boop, 220,100);
}
void _B1(){
  tone(boop, 246,100);
}
void _C1(){
  tone(boop, 261,100);
}
void _D1(){
  tone(boop, 293,100);
}
void _E1(){
  tone(boop, 329,100);
}
void _F1(){
  tone(boop, 349,100);
}
void _G1(){
  tone(boop, 391,100);
}
void _A2(){
  tone(boop, 440,100);
}
void _B2(){
  tone(boop, 493,100);
}
void _C2(){
  tone(boop, 523,100);
}
void _D2(){
  tone(boop, 587,100);
}
void _E2(){
  tone(boop, 659,100);
}
void _F2(){
  tone(boop, 698,100);
}
void _G2(){
  tone(boop, 783,100);
}
