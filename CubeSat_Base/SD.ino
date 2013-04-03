
boolean SD_init() {
  pinMode(SCK,OUTPUT);
  pinMode(MOSI,OUTPUT);
  pinMode(10, OUTPUT); 

  if(!SD.begin(SD_CS)){
    Serial.println(F("SD: ERROR"));
    Error();
  }
}




