char xbuf[32];

void XBee_init(){
  pinMode(XB_ON,INPUT);
  pinMode(XB_CTS,INPUT);
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

  delay(1100);
  XBee.print("+++");
  XBee.flush();
  delay(1100);
  rcv = XBee.readBytesUntil('\r',xbuf,32);

  return rcv==2 ? true:false;
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

void XBee_discardBuffer(){
  while(XBee.available())
    XBee.read();
}

boolean XBee_CTS(){
  return !digitalRead(XB_CTS);
}

void XBee_waitForCTS(){
  while(!XBee_CTS()){
#ifdef DEBUG_ON
    Serial.print(F("CTS"));
#endif
    delay(10);
  }
}


