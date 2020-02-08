#ifndef DBOX_RELE
#define DBOX_RELE

#include "Arduino.h"

#define PIN_RELE D6

int rele_state = LOW;

void setupRele() {
  pinMode(PIN_RELE, OUTPUT);
  delay(100);
}

void switchState() {
  if (rele_state == LOW) {
    rele_state = HIGH;
  } else {
    rele_state = LOW;
  }
}

void updateRele() {
  digitalWrite(PIN_RELE, rele_state);
}

#endif