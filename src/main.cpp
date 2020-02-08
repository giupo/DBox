#include <Arduino.h>
#include <Wire.h>

#include <DS3231.h>
#include "DHT.h"
#include "LiquidCrystal_I2C.h"

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;
#define I2C_LCD_ADDRESS  0x3F
#define I2C_RTC_ADDRESS  0x68

LiquidCrystal_I2C lcd(I2C_LCD_ADDRESS, lcdColumns, lcdRows); 
DS3231 c;

char Time[]        = "       :  :  ";
char Calendar[]    = "       /  /20  ";
char Temperatura[] = "Temp:   ";

#define DHTTYPE DHT11
#define DHT_PIN D8

DHT dht(DHT_PIN, DHTTYPE);
float temp;
float humid;


void setupDH11() {
  pinMode(DHT_PIN, INPUT);
  dht.begin();
}

void setupDisplay() {
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
}

void setupI2C() {
  Wire.begin();
}

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

void printDHT() {
  lcd.setCursor(0, 3);
  lcd.print(temp);
  lcd.print(" ");
  lcd.print(humid);
}

void printTime() {
  bool Century    = false;
  bool PM = false, h12 = false;

  int second      = c.getSecond(),
      minute      = c.getMinute(),
      hour        = c.getHour(h12, PM),
      date        = c.getDate(),
      month       = c.getMonth(Century),
      year        = c.getYear(),
      temperatura = c.getTemperature();

  
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

  Temperatura[7] = temperatura % 10 + 48;
  Temperatura[6] = temperatura / 10 + 48;

  lcd.setCursor(0, 0);
  lcd.print(Time);
  lcd.setCursor(0, 1);
  lcd.print(Calendar);
  lcd.setCursor(5, 2);
  lcd.print(Temperatura);
  lcd.setCursor(13, 2);
  lcd.print((char)223);
}

void updateDisplay() {
  lcd.clear();
  printTime();
  printDHT();
}

void updateDHT() {
  temp = dht.readTemperature(); // Gets the values of the temperature
  humid = dht.readHumidity(); // Gets the values of the humidity 
}

# if 0

void setupScanI2C() {
  Wire.begin();
  Serial.begin(115200);
  Serial.println("\nI2C Scanner");
}

void scanI2C() { 
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
      nDevices++;
    }
    else if (error==4) {
      Serial.print("Unknow error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("done\n");
  }
  delay(5000);          
}
#endif

void setup() {
  setupI2C();
  setupRTC();
  setupDisplay();  
  //setupScanI2C();
}

void loop() {
  // scanI2C();
  updateDHT();
  updateDisplay();
  delay(1000);
}