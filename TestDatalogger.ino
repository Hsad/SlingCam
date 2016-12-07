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
int storeSize = 1000;
float baromStore[1000];  //Array of last alt

//Countdown beeps
long Countdown = 0;
long CountdownInterval = 300;
int overflower = 0;

//loop timer
//unsigned long lastTime = 0;
//unsigned long loopDelayTime = 10;
//unsigned long loopDiffStore;

void setup() {
  //tripleFlash();
  // Open serial communications and wait for port to open:
  //Serial.begin(9600);
  //while (!Serial) {
  //  ; // wait for serial port to connect. Needed for native USB port only
  //}
  tone(boop, 660, 100);
  delay(350);
  //myservo.attach(servopin);
  //set servo to correct location.
  //myservo.write(90);
  tone(boop, 660, 100);
  delay(350);
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
  tone(boop, 660, 100);
  delay(350);
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
    dataFile.println("Running/..,.,.,.,");
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

  

  flash(2, red);
  flash(2, blue);
  flash(2, green);
  tone(boop, 660, 100);
  */

  
}


//need some system so that the logging can occur while there is a three beep countdown
//need the logging to continue after the chute comes out
//need to record in the log the moment of chute release, give me an idea of when it should happen based on barometer data.
void loop() {
  // make a string for assembling the data to log:
  //String dataString = "Hello Dash";

  Countdown++;
  if (Countdown % 100 == 0){  //just a debug
    overflower++;
  }
  if (Countdown == CountdownInterval or Countdown == CountdownInterval*2 or Countdown == CountdownInterval*3){
    //if that aint the dirtiest line of code I've ever written well I'll be
    beep(400);   
  }
  if (Countdown == CountdownInterval*4){
    //un cork, 
    delay(1000);
    myservo.write(145);
    //and log it 
    File dataFile = SD.open("datalog2.txt", FILE_WRITE);
    // if the file is available, write to it:
    if (dataFile) {
      dataFile.println("Deploy");
      dataFile.close();
    }
    // if the file isn't open, pop up an error:
    else {
      beep(350);    
    }
  }
  if (Countdown == CountdownInterval*10){
    //reset location after a bit.
    myservo.write(90);
    delay(1000); //delay becasue everything should have happened by now.... Though that may change when the camera is added
  }


  //////Store data, speed boosted, will lead to a stutter in the data points though...
  baromStore[Countdown % storeSize] = bme.readPressure();
  if (Countdown+1 % storeSize == 0){ //if the next loop is the 101st
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File dataFile = SD.open("datalog2.txt", FILE_WRITE);
    // if the file is available, write to it:
    if (dataFile) {
      for (int B_ind = 0; B_ind < storeSize; B_ind++){
        dataFile.println(baromStore[B_ind]);
      }
      //dataFile.println(bme.readPressure());
      dataFile.close();
    }
    // if the file isn't open, pop up an error:
    else {
      beep(350);    
    }
  }

  //TODO
  /*Calculate how long it takes the barometer call to respond, then how long it takes to write a line*/
  /*
  //track the time frame
  //and delay appropriatly
  loopDiffStore = millis()-lastTime; //becasue there is a really tiny chance this could underflow in the time that elapses, causing a massive delay, like 50 days.
  if (loopDiffStore < loopDelayTime){    
    delay(loopDelayTime - loopDiffStore); //m-t = time elapsed this loop, subract from ideal loop time
  }
  
  lastTime = millis();
  */
}


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

