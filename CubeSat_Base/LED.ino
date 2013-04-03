void LED_init() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED,LOW);
}

void LED_on() {
  digitalWrite(LED,HIGH);
}

void LED_off() {
  digitalWrite(LED,LOW);
}

void LED_flash(int time) {
  LED_on();
  delay(time);
  LED_off();
  delay(time);
}

void Error() {
  while(true)
    LED_flash(100);
}


