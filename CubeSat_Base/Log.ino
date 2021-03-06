
void Log_analog() {
  // Timestamp
  RTC_timestamp();
  Serial.print(F("Log: "));
  Serial.println(timestamp);

  if(clearLog){
    clearLog=false;
    SD.remove("pres.csv");
    SD.remove("temp1.csv");
    SD.remove("temp2.csv");
    Serial.println(F("Logs Cleared"));
  }

  // Update sensor readings
  Sensor_update();

  //Pressure
  File f_pres = SD.open("pres.csv", FILE_WRITE);
  if(f_pres){
    f_pres.print(timestamp);
    f_pres.print(',');
    f_pres.print(pres,DEC);
    f_pres.print(',');
    f_pres.println(r_pres,DEC);
  } 
  else {
    Serial.println(F("File Error"));
  }
  f_pres.close();

  //Temp1
  File f_temp1 = SD.open("temp1.csv", FILE_WRITE);
  if(f_temp1){
    f_temp1.print(timestamp);
    f_temp1.print(',');
    f_temp1.print(temp1,DEC);
    f_temp1.print(',');
    f_temp1.println(r_temp1,DEC);
  } 
  else {
    Serial.println(F("File Error"));
  }
  f_temp1.close();

  //Temp2
  File f_temp2 = SD.open("temp2.csv", FILE_WRITE);
  if(f_temp2){
    f_temp2.print(timestamp);
    f_temp2.print(',');
    f_temp2.print(temp1,DEC);
    f_temp2.print(',');
    f_temp2.println(r_temp1,DEC);
  } 
  else {
    Serial.println(F("File Error"));
  }
  f_temp2.close();
}
