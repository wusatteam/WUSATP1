// Receive buffer
char xbuf[32];

// Initialise XBee
void XBee_init(){
  pinMode(XB_ON,INPUT);
  pinMode(XB_CTS,INPUT);
  pinMode(XB_RST,INPUT);
  xbeeDetected = digitalRead(XB_ON); 

  if(!xbeeDetected)
    Serial.println(F("XBEE: Not Detected"));
  else{

#ifdef DEBUG_ON
    Serial.println(F("XBEE: Detected"));
#endif

    XBee.begin(115200);
    XBee.setTimeout(3000);  
  }

}


// Enter command mode
boolean XBee_enterCommand() {
  int rcv=0;

  XBee.flush();
  XBee_discardBuffer();  

  delay(1100); //Guard time
  XBee.print("+++"); //Command mode
  XBee.flush(); //Guard time
  delay(1100);
  rcv = XBee.readBytesUntil('\r',xbuf,32);

  return rcv==2 ? true:false; //'OK' expected
}

// Exit command mode
boolean XBee_exitCommand(){
  XBee_discardBuffer();
  XBee.print("ATCN\r");
  int rcv = XBee.readBytesUntil('\r',xbuf,32);
  return rcv==2 ? true:false;
}

// Discovers node then exits command mode to allow immediate communication
boolean XBee_discoverNode(){
  XBee_discardBuffer();
  XBee.setTimeout(10000);
  XBee.print("ATDN");
  XBee.print(NODE);
  XBee.print('\r');

  int rcv = XBee.readBytesUntil('\r',xbuf,32);
  XBee.setTimeout(3000);
  return rcv==2 ? true:false;
}

// Empty received buffer
void XBee_discardBuffer(){
  while(XBee.available())
    XBee.read();
}

// Check status of the CTS line
boolean XBee_CTS(){
  return !digitalRead(XB_CTS);
}

// Wait until clear to send data
void XBee_waitForCTS(){
  int count=0;
  while(!XBee_CTS() && count<100){
#ifdef DEBUG_ON
    Serial.print(F("CTS"));
#endif
    delay(10);
    count++;
  }
  if(count>=100){
    XBee_reset();
  }
  
}

// Force the XBee to reset by driving XB_RST low
void XBee_reset(){
  pinMode(XB_RST,OUTPUT); //Drives low by default
  delay(5); //100us minimum
  pinMode(XB_RST,INPUT); //High impedance
  delay(10);
}
