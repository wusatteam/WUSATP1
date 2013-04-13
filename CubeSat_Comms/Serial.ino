// Initialise Serial Port
void Serial_init() {
  Serial.begin(38400);

#ifdef OPTION_SERIAL_WAIT
  // Wait for Arduino Leonard USB-UART to connect
  while(!Serial){
  }
#endif
}

// Process received command from CMD_PORT
void Serial_command(){
  char cmd[3];
  int clen = CMD_PORT.readBytes(cmd,3);

  // Parse command
  switch(cmd[0]){
  case 'S':
    if(cmd[1]=='D') Cmd_shutdown(); //SDN
    if(cmd[1]=='E') Cmd_set(); //SET
    break;
  case 'P': //PAU
    Cmd_pause();
    break;
  case 'R':
    Cmd_reset(); //RST
    break;
  default:
    // No command
    break;   
  }

  // Clear Rx buffer 
  while(CMD_PORT.available()>0)
    CMD_PORT.read();
}

// Print string with timestamp
void DEBUGLN(char* str){
#ifdef DEBUG_ON
  Serial.print(millis(),DEC);
  Serial.print(": ");
  Serial.println(str);
#endif
}

// Shutdown - currently just stops execution
void Cmd_shutdown(){
  CMD_PORT.println(F("SDN"));
  Error();
}

// Set date and time, e.g. SETMar 06 201312:34:00
void Cmd_set(){
  char time[8];
  char date[11];
  CMD_PORT.readBytes(date,11);
  CMD_PORT.readBytes(time,8);
  RTC.adjust(DateTime(date,time));
  CMD_PORT.println(F("D/T Updated."));
}

// Clear log files
void Cmd_reset(){
  clearLog=true;
}

// Pause execution
void Cmd_pause(){
  CMD_PORT.println("PAUSE");
  char cmd[3];
  // Wait until 3 characters are received
  while(CMD_PORT.readBytes(cmd,3)<3)
    ;
}

// Print Header and build information
void Serial_header(){
  Serial.println(F("CubeSat FW - Comms"));
  Serial.print(F("V "));
  Serial.print(__DATE__);
  Serial.print(' ');
  Serial.println(__TIME__);
#ifdef DEBUG_ON
  Serial.println(F("\tDBG ON"));
#endif
}


