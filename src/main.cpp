#include <main.h>

// main application object
application_structures progress_timer;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  application_init(&progress_timer);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Hello MEHDI"); delay(1000);
}