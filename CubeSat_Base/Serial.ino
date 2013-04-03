void Serial_init() {
  Serial.begin(38400);

#ifdef OPTION_SERIAL_WAIT
  while(!Serial){
  }
#endif
}

void Serial_command(){
  char cmd[3];
  int clen = CMD_PORT.readBytes(cmd,3);

  // Parse command
  switch(cmd[0]){
  case 'S':
    if(cmd[1]=='D') Cmd_shutdown(); //SDN
    if(cmd[1]=='E') Cmd_set(); //SET
    break;
  case 'P':
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


void DEBUGLN(char* str){
#ifdef DEBUG_ON
  Serial.print(millis(),DEC);
  Serial.print(": ");
  Serial.println(str);
#endif
}

void Cmd_shutdown(){
  CMD_PORT.println(F("SDN"));
  Error();
}

void Cmd_set(){
  char time[8];
  char date[11];
  CMD_PORT.readBytes(date,11);
  CMD_PORT.readBytes(time,8);
  RTC.adjust(DateTime(date,time));
  CMD_PORT.println(F("D/T Updated."));
}

void Cmd_reset(){
  clearLog=true;
}

void Cmd_pause(){
  CMD_PORT.println("PAUSE");
  char cmd[3];
  while(CMD_PORT.readBytes(cmd,3)<3)
    ;
}

void Serial_header(){
  // Header and build information
  Serial.println(F("CubeSat FW - Comms"));
  Serial.print(F("V "));
  Serial.print(__DATE__);
  Serial.print(' ');
  Serial.println(__TIME__);
#ifdef DEBUG_ON
  Serial.println(F("\tDBG ON"));
#endif
}


