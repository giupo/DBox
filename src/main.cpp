#include <Arduino.h>
#include "i2c_utils.hpp"
#include "dbox_rtc.hpp"
#include "dbox_display.hpp"
#include "dbox_dht.hpp"
#include "dbox_rele.hpp"


void setup() {
  setupI2C();
  #ifdef DEBUG
  setupSerial();
  #endif
  setupRTC();
  setupDisplay();
  setupRele();
  setupDH11();
}

void update() {
  updateRTC();
  updateDHT();
  updateRele();
}

void render() {
  printTime(second, minute, hour, year, month, date);
  printDHT(temp, humid);  
}

void loop() {
  // scanI2C();
  update();
  render();
  //switchState();
  delay(1000);
}