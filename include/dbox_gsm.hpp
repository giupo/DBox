#ifndef DBOX_GSM
#define DBOX_GSM

#include <SoftwareSerial.h>
#include "PString.h"
// Configure software serial port
#define GSM_TX_PIN D7
#define GSM_RX_PIN D8

SoftwareSerial SIM900(D7, D8); 

void setupGSM() {
  // Arduino communicates with SIM900 GSM shield at a baud rate of 19200
  // Make sure that corresponds to the baud rate of your module
  SIM900.begin(19200);
  // Give time to your GSM shield log on to network

  // AT command to set SIM900 to SMS mode
  SIM900.print("AT+CMGF=1\r"); 
  delay(100);
  // Set module to send SMS data to serial out upon receipt 
  SIM900.print("AT+CNMI=2,2,0,0,0\r");    
}

void sendSMS(String number, String msg) {
  // AT command to set SIM900 to SMS mode
  SIM900.print("AT+CMGF=1\r"); 
  delay(100);

  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  SIM900.println("AT + CMGS = \"+"+String(number)+"\""); 
  delay(100);
  
  // REPLACE WITH YOUR OWN SMS MESSAGE CONTENT
  SIM900.println(msg); 
  delay(100);

  // End AT command with a ^Z, ASCII code 26
  SIM900.println((char)26); 
  delay(100);
  SIM900.println();
  // Give module time to send SMS
  delay(5000); 
}

String readSMS() {
// Display any text that the GSM shield sends out on the serial monitor
  if(SIM900.available() >0) {        
    return SIM900.readString();
  }
  return "";
}

#endif