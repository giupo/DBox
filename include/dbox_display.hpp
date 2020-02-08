#ifndef DBOX_DISPLAY
#define DBOX_DISPLAY

#include "Wire.h"
#include "LiquidCrystal_I2C.h"
#include "dbox_version.hpp"
#define I2C_LCD_ADDRESS  0x3F

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 4;

char Time[]        = "       :  :  ";
char Calendar[]    = "       /  /20  ";
char Temperatura[] = "Temp:   ";
 
LiquidCrystal_I2C lcd(I2C_LCD_ADDRESS, lcdColumns, lcdRows); 

void setupDisplay() {
  // initialize LCD
  lcd.init();
  lcd.begin(lcdColumns , lcdRows);
  // turn on LCD backlight                      
  lcd.backlight();
}

void printTime(int second, int minute, int hour, int year, int month, int date) {
  Time[12]     = second % 10 + 48;
  Time[11]     = second / 10 + 48;
  Time[9]      = minute % 10 + 48;
  Time[8]      = minute / 10 + 48;
  Time[6]      = hour   % 10 + 48;
  Time[5]      = hour   / 10 + 48;
  Calendar[14] = year   % 10 + 48;
  Calendar[13] = year   / 10 + 48;
  Calendar[9]  = month  % 10 + 48;
  Calendar[8]  = month  / 10 + 48;
  Calendar[6]  = date   % 10 + 48;
  Calendar[5]  = date   / 10 + 48;

  //Temperatura[7] = temperatura % 10 + 48;
  //Temperatura[6] = temperatura / 10 + 48;

  lcd.setCursor(1, 0);
  lcd.print(Time);
  lcd.setCursor(0, 1);
  lcd.print(Calendar);
  lcd.setCursor(5, 2);
  //lcd.print(Temperatura);
  //lcd.setCursor(13, 2);
  
}


void printDHT(float temp, float humid) {
  lcd.setCursor(4, 3);
  lcd.print(temp);
  lcd.print((char)223);
  lcd.print(" ");
  lcd.print(humid);
}

#ifdef DEBUG
void printHelloWorld() {
  lcd.setCursor(0, 0);
  lcd.print("Hello World");
}
#endif

void printSplash(int timeDelay) {
  lcd.clear();
  lcd.setCursor(0, 2);
  lcd.print("DBox");
  lcd.print(", v");
  lcd.print(VERSION);
  lcd.setCursor(0, 3);
  lcd.print("by Giupao");
  delay(timeDelay);
  lcd.clear();
}

#endif