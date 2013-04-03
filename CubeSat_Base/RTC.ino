
void RTC_init() {
  Wire.begin();
  RTC.begin();

  if (! RTC.isrunning()) {
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
    
#ifdef DEBUG_ON 
    Serial.println(F("\tRTC: Backup Battery Failed")); 
#endif

  }
}

void RTC_timestamp(){
  //  char timestamp[]="2010/00/00 00:00:00";
  DateTime now = RTC.now();

  // Year
  timestamp[3] = '0' + now.year() % 10;
  // Month
  timestamp[5] = '0' + now.month() / 10;
  timestamp[6] = '0' + now.month() % 10;
  // Day
  timestamp[8] = '0' + now.day() / 10;
  timestamp[9] = '0' + now.day() % 10;

  // Hour
  timestamp[11] = '0' + now.hour() / 10;
  timestamp[12] = '0' + now.hour() % 10;
  // Minute
  timestamp[14] = '0' + now.minute() / 10;
  timestamp[15] = '0' + now.minute() % 10;
  // Second
  timestamp[17] = '0' + now.second() / 10;
  timestamp[18] = '0' + now.second() % 10;
}


