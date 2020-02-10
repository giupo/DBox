#include "Arduino.h"
#include "dbox_state.hpp"
#include "dbox_display.hpp"
#include "EEPROM.h"

#include <ESP8266WiFi.h>

State::State() {
}

void State::init() {
  EEPROM.begin(512);
  EEPROM.get(EEPROM_ADDR, data);
  EEPROM.end();

  if (data.state != NOT_CONFIGURED || data.state != CONFIGURED) {
    data.state = NOT_CONFIGURED;
  }
}

void State::save() {
  EEPROM.begin(512);
  EEPROM.put(EEPROM_ADDR, data);
  EEPROM.commit();  

  if( !EEPROM.commit() ) {
    #ifdef DEBUG
    Serial.println("Failed to write on EEPROM");
    #endif    
    printMsg("Err: Can't save state");   
  }
}

void State::saveWiFiCredentials(const String ssid, const String pwd) {
  bool changed = false;
  if (String(data.wifi_ssid) != ssid) {
    strncpy(data.wifi_ssid, ssid.c_str(), ssid.length());
    changed = true;
  }

  if (String(data.wifi_pwd) != String(pwd)) {
    strncpy(data.wifi_pwd, pwd.c_str(), pwd.length());
    changed = true;
  }
   
  if(changed) {
    data.state = CONFIGURED;
    save();
  }
}

State::DBOX_state State::getState() {  
  return data.state;  
}


void State::setState(DBOX_state state) {
  dbox_state = state;
  save();
}

String State::getSSID() {
  String x = data.wifi_ssid;
  x.trim();
  return x;
}

String State::getWifiPassword() {
  String x = data.wifi_pwd;
  x.trim();
  return x;
}

void State::print() {
  printMsg(message);
}


void State::update() {
if (data.state == NOT_CONFIGURED) {
    message = "  IP: " + WiFi.softAPIP().toString();
  } else if(data.state == CONFIGURED) {
    message = "  IP: " + WiFi.localIP().toString();    
  }   
}

String State::get_message() {
  return message;
}


extern State state;

void setupState() {
  state.init();
}
