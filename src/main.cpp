#include <Arduino.h>
#include "dbox_i2c.hpp"
#include "dbox_rtc.hpp"
#include "dbox_display.hpp"
#include "dbox_dht.hpp"
#include "dbox_rele.hpp"
#include "dbox_state.hpp"

#ifdef GSM
#include "dbox_gsm.hpp"
#endif

#ifdef WIFI
#include "dbox_wifi.hpp"  
#endif

// main state of application
State state;

void setup() {
  setupI2C();
  setupDisplay();
  setupState();
  printSplash(2000);

  #ifdef GSM
  setupGSM();    
  printWithDelayAndDots(String("init GSM"), 20000);
  #endif

  #ifdef WIFI
  setupWifi();
  #endif

  #ifdef DEBUG
  setupSerial();
  #endif
  setupRTC();
  setupRele();
  setupDH11();
  clearDisplay();
}

void update() {
  updateRTC();
  updateDHT();
  updateRele();
  state.update();
}

void render() {
  printTime(second, minute, hour, year, month, date);
  printDHT(getTemperature(), getHumidity());
  state.print();
}

void loop() {
  // scanI2C();
  update();
  render();
  //switchState();
  delay(500);
}