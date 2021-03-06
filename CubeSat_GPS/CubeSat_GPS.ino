//#define DEBUG_ON //Approx 660 bytes
#define CAM_RX     11
#define CAM_TX     12
#define LED        13
#define SD_CS      4
#define PRES       A0
#define TEMP1      A2
#define TEMP2      A1
#define VOLT       A3
#define CURR       A4
#define CAM_KH     0x00
#define CAM_KL     0xF0
#define CAM_XH     0x00
#define CAM_XL     0x0A
#define BLK        ((CAM_KH<<8)|CAM_KL) //Outer paretheses necessary for precedence reasons

//Options
#define OPTION_FLASH
//#define OPTION_SET_RES
//#define OPTION_RAM_CHECK //Approx 224 bytes
//#define OPTION_SERIAL_WAIT
#define CMD_PORT  Serial
#define GPS       Serial1

// RTClib must come after Wire
#include <Wire.h>
#include <SD.h>
#include <RTClib.h>
#include <SoftwareSerial.h>

// Devices
RTC_DS1307 RTC;
SoftwareSerial Cam(CAM_RX,CAM_TX);

// Global Variables
int r_pres=0,r_temp1=0,r_temp2=0,r_volt=0,r_curr=0;
int pres=0,temp1=0,temp2=0,volt=0,curr=0;
char timestamp[]="2010/00/00 00:00:00";
boolean clearLog=false;

void setup() {

  // Initialise USB serial port - Debug
  Serial_init();

  // Header
  Serial_header();

  // Set up port directions
  Sensor_init();
  LED_init();

  // Initialise SD card
  SD_init();

  // Initialise Real Time Clock - includes setting time and date to build time/date
  RTC_init();
  RTC_timestamp();

  GPS_init();

  // Connect to camera and reset
  Cam_init();

#ifdef OPTION_SET_RES
  // Set 640x480 resolution
  if(!Cam_setRes()){
    Serial.println(F("Set Resolution Failed"));
    Error();
  }
#endif

  //DEBUGLN("Boot Complete.");
  Serial.print(F("Started "));
  Serial.println(timestamp);
}

void loop() {
  Serial.println(F("=== Main FW Loop ==="));
  Ram_check();
  
  // Process waiting command
  Serial_command();

  // Log analog readings
  Log_analog();
  
  // Log GPS
  Log_GPS();

  // Take picture
  Cam_capture();

  // Output file size
  unsigned int sz = Cam_size();
  Serial.print(F("Picture size: "));
  Serial.print(sz,DEC);
  Serial.println("bytes");


  //DEBUGLN("Downloading Picture to SD Card...");
  Cam_download();
}


void Ram_check(){
#ifdef OPTION_RAM_CHECK
  int f = freeRam();
  int p = (f*100)/2500;
  Serial.print(F("RAM: "));
  Serial.print(freeRam(),DEC);
  Serial.print(F("/2500 "));
  Serial.print(p,DEC);
  Serial.println(F("% free"));
#endif
}

#ifdef OPTION_RAM_CHECK
int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
#endif















