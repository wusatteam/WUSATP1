// Buffer sized to handle block transfer
// Requires modification to SoftwareSerial.h
//  - #define _SS_MAX_RX_BUFF 256

char res[BLK];
int rcv = 0;

void Cam_init() {
  Cam.begin(38400);
  Cam.setTimeout(5000);

#ifdef DEBUG_ON 
  Serial.print(F("\tCAM: RX B: ")); 
  Serial.println(_SS_MAX_RX_BUFF,DEC);
  Serial.print(F("\tCAM: BLK: ")); 
  Serial.println(BLK,DEC);
#endif

  if(_SS_MAX_RX_BUFF<256)
    Serial.println(F("RX B Error"));

  if(!Cam_reset()){
    Serial.println(F("Camera Error"));
    Error();
  }
}

boolean Cam_reset() {
  const char RESET[4] = {
    0x56,0x00,0x26,0x00                                                                              };
  Cam_command(RESET,4);

  rcv = Cam.readBytes(res,71); //71
  return rcv == 71;
}

void Cam_capture() {
  const char CAPTURE[5] = {
    0x56,0x00,0x36,0x01,0x00                                                                              };

#ifdef OPTION_FLASH
  LED_on();
  delay(200);
#endif

  Cam_command(CAPTURE,5);

  rcv = Cam.readBytes(res,5); //5
  if(rcv<5){
    Serial.println("Capture Error");
    Error();
  }

#ifdef OPTION_FLASH
  LED_off();
#endif
}

unsigned int Cam_size() {
  const char SIZE[5]={
    0x56,0x00,0x34,0x01,0x00                                                    };
  Cam_command(SIZE,5);

  rcv = Cam.readBytes(res,9);
  if(rcv<9){
    Serial.println(F("Size Error"));
    Error();
  }
  else{
    byte XH = res[7];
    byte XL = res[8];

    return XL | (XH << 8);
  }
}

void Cam_setRes(){
  const char RES[9] = {
    0x56,0x00,0x31,0x05,0x04,0x01,0x00,0x19,0x00                                                    };
  Cam_command(RES,9);

  rcv=Cam.readBytes(res,5); //5
  if(rcv==5){
    Cam_reset();
  } 
  else {
    Serial.println(F("Set Resolution Failed"));
    Error();
  }
}

void Cam_download() {
  const char READ[8] = {
    0x56,0x00,0x32,0x0C,0x00,0x0A,0x00,0x00                            };

  long addr=0x0000;
  long total=0;
  boolean EndOfFile = false;
  char fname[] = "00000000.JPG";
  DateTime now = RTC.now();

  // Generate filename from time
  fname[0] = '0' + now.day()/10;
  fname[1] = '0' + now.day()%10;
  fname[2] = '0' + now.hour()/10;
  fname[3] = '0' + now.hour()%10;
  fname[4] = '0' + now.minute()/10;
  fname[5] = '0' + now.minute()%10;
  fname[6] = '0' + now.second()/10;
  fname[7] = '0' + now.second()%10;
  Serial.print(F("Filename: "));
  Serial.println(fname);

  // Remove existing picture if necessary
  if(SD.exists(fname)){
    SD.remove(fname);
  }

  // Open file
  File pic = SD.open(fname, FILE_WRITE);
  if(!pic){
    Serial.println(F("File Error"));
    Error();
  }
  if(xbeeDetected && xbeeDiscovered)
    XBee_waitForCTS();
    XBee.print("APICT:");

  // Download picture
  while(!EndOfFile) {

#ifdef DEBUG_ON
    Serial.print(F("\tA: "));
    Serial.print(addr,HEX);
#endif

    Cam_command(READ,8);
    Cam.write((uint8_t)(addr/0x100));
    Cam.write((uint8_t)(addr%0x100));
    Cam.write((uint8_t)0x00);
    Cam.write((uint8_t)0x00);
    Cam.write((uint8_t)CAM_KH);
    Cam.write((uint8_t)CAM_KL);
    Cam.write((uint8_t)CAM_XH);
    Cam.write((uint8_t)CAM_XL);

    // Discard 5
    rcv=Cam.readBytes(res,5);
    if(rcv<5){
      Serial.println(F("Response Error"));
      Error();
    }

    //DEBUGLN("Waiting for response");
    rcv = Cam.readBytes(res,BLK);

#ifdef DEBUG_ON
    Serial.print(F("\tR: "));
    Serial.print(rcv,DEC);
#endif

    if(rcv<BLK){
      Serial.print(F("Block Error: "));
      Serial.println(BLK,DEC);
      Serial.print("Rcv: ");
      Serial.print(rcv,DEC);
      Error();
    }

    //Write first 2 bytes
    pic.write(res[0]);
    pic.write(res[1]);
    if(xbeeDetected && xbeeDiscovered){
      XBee_waitForCTS();
      XBee.write(res[0]);
      XBee_waitForCTS();
      XBee.write(res[1]);
    }
    // Write remaining data to file unless end of file is reached
    int count=2;
    for(int i=2;i<BLK;i++){
      if((byte)res[i-2]==0xFF && (byte)res[i-1]==0xD9){
        EndOfFile=true;
        break;
      } 
      else {
        pic.write(res[i]);
        if(xbeeDetected && xbeeDiscovered){
          XBee_waitForCTS();
          XBee.write(res[i]);
        }
        count++;
      }
    }

#ifdef DEBUG_ON
    Serial.print(F("\tW: "));
    Serial.print(count,DEC);
    total+=count;
    Serial.print(F("\tT: "));
    Serial.println(total,DEC);
#endif

    pic.flush();

    if(xbeeDetected && xbeeDiscovered){
      XBee.flush();
      //delay(100);
    }
    
    addr+=BLK;
  }

#ifdef DEBUG_ON
  Serial.print(F("\tP: "));
  Serial.print(total,DEC);
  Serial.println("b");
#endif

  Cam_stop();
  Cam.flush();
  
  pic.close();
  
  if(xbeeDetected){
    XBee_waitForCTS();
    XBee.print("\r\n");
    XBee.flush();
  }
  
}

void Cam_stop(){
  const char STOP[5] = {
    0x56,0x00,0x36,0x01,0x03                          };
  Cam_command(STOP,5);

  rcv = Cam.readBytes(res,5); //5
  if(rcv<5){
    Serial.println(F("Capture Error"));
    Error();
  }
}

void Cam_command(const char* command,int length) {
  // Flush Rx buffer
  Cam.flush();
  // Send command string
  for(int i=0; i < length; i++) {
    Cam.write((uint8_t)command[i]);
  }
}

















