
void Sensor_init(){
  pinMode(PRES,INPUT);
  pinMode(TEMP1,INPUT);
  pinMode(TEMP2,INPUT);
  //pinMode(VOLT,INPUT);
  //pinMode(CURR,INPUT);
}

void Sensor_update() {
  r_pres = analogRead(PRES);
  r_temp1 = analogRead(TEMP1);
  r_temp2 = analogRead(TEMP2);
  //r_volt = analogRead(VOLT);
  //r_curr = analogRead(CURR);

  pres = map(r_pres,0,1023,0,5000);
  temp1 = map(r_temp1,0,1023,0,5000);
  temp2 = map(r_temp2,0,1023,0,5000);
  //volt = map(r_volt,0,1023,0,5000);
  //curr = map(r_curr,0,1023,0,5000);

#ifdef DEBUG_ON
  Serial.print(F("\tPRES: "));
  Serial.println(pres,DEC);
  Serial.print(F("\tTEMP1: "));
  Serial.println(temp1,DEC);
  Serial.print(F("\tTEMP2: "));
  Serial.println(temp2,DEC);
#endif
}
