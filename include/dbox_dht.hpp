#ifndef DBOX_DHT
#define DBOX_DHT

// e' molto importante che questo include...
#include <Adafruit_Sensor.h>
// preceda questo: 
#include "DHT.h"
// da https://github.com/adafruit/DHT-sensor-library/issues/62

//#include "DHTesp.h"

#define DHT_TYPE DHT11
#define DHT_PIN D5
#define DHT_COUNT
//DHTesp dht;

DHT dht(DHT_PIN, DHT_TYPE);
float temp = 0.0;
float humid = 0.0;


void setupDH11() {
  dht.begin();
  //dht.setup(DHT_PIN, DHTesp::DHT11); // Connect DHT sensor to GPIO 17
}

void updateDHT() {
  temp = dht.readTemperature(); // Gets the values of the temperature
  humid = dht.readHumidity(); // Gets the values of the humidity
  #ifdef DEBUG
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print(", ");
  Serial.print("Humidity: ");
  Serial.println(humid);
  #endif
}
     
     

#endif