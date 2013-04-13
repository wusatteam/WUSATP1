// SD Initialisation
void SD_init() {
  pinMode(SCK,OUTPUT);
  pinMode(MOSI,OUTPUT);
  pinMode(10, OUTPUT); 

  if(!SD.begin(SD_CS)){
    Serial.println(F("SD Error"));
    Error();
  }
}




