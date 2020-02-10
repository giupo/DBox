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

void setupDH11();
void updateDHT();
     
float getTemperature();
float getHumidity();

#endif