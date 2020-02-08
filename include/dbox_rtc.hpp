#ifndef DBOX_RTC
#define DBOX_RTC

#include <Wire.h>

#include <DS3231.h>
#include "PString.h"
#define I2C_RTC_ADDRESS  0x68

DS3231 c;

bool Century    = false,
     PM         = false, 
     h12        = false;

int second      = 0,
    minute      = 0,
    hour        = 0,
    date        = 0,
    month       = 0,
    year        = 0,
    temperatura = 0;


void setupRTC() {
  #if 0
  c.setSecond(50);
  c.setMinute(20);
  c.setHour(19);
  c.setDate(7);
  c.setMonth(2);
  c.setYear(20);
  #endif
}

void updateRTC() {
  second      = c.getSecond(),
  minute      = c.getMinute(),
  hour        = c.getHour(h12, PM),
  date        = c.getDate(),
  month       = c.getMonth(Century),
  year        = c.getYear(),
  temperatura = c.getTemperature();

  #ifdef DEBUG
  char buf[10];
  char sep=':';
  PString mystring(buf, sizeof(buf));
  if(hour<10) mystring.print('0');
  mystring.print(hour);
  mystring.print(sep);

  if(minute<10) mystring.print('0');
  mystring.print(minute);
  mystring.print(sep);
  
  if(second<10) mystring.print('0');
  mystring.print(second);
  
  Serial.println(buf); 
  #endif
}

#endif