// Initialise LED pin mode
void LED_init() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED,LOW);
}

// LED on
void LED_on() {
  digitalWrite(LED,HIGH);
}

// LED off
void LED_off() {
  digitalWrite(LED,LOW);
}

// Flash LED at specific interval indefinitely
void LED_flash(int time) {
  LED_on();
  delay(time);
  LED_off();
  delay(time);
}

// Error indicator
void Error() {
  while(true)
    LED_flash(100);
}


