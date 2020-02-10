#ifndef DBOX_DISPLAY
#define DBOX_DISPLAY

#define I2C_LCD_ADDRESS  0x3F

#include "Arduino.h"

void setupDisplay();
void printTime(int second, int minute, int hour, int year, int month, int date);
void printDHT(float temp, float humid);

#ifdef DEBUG
void printHelloWorld();
#endif

void printSplash(int timeDelay);
void printMsg(String msg);
void printAndDelay(String stringMessage, uint delayTime);
void printWithDelayAndDots(String msg, uint delayTime);

void clearDisplay();

#endif