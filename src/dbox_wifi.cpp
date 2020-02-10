#include "dbox_wifi.hpp"

#include "dbox_state.hpp"
#include "dbox_display.hpp"
#include "dbox_dht.hpp"

#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

extern State state;
int wifi_status = WL_IDLE_STATUS;

AsyncWebServer server(80);


// Replaces placeholder with DHT values
String processor(const String& var) {
  if(var == "TEMPERATURE"){
    return String(getTemperature());
  }
  else if(var == "HUMIDITY"){
    return String(getHumidity());
  }
  return String();
}

const char index_html[] PROGMEM = R"rawliteral(
<!doctype html><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>DBOX Server</h2>
  <p>
    <span class="dht-labels">Temperature</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <span class="dht-labels">Humidity</span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">%</sup>
  </p>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 10000 ) ;
</script>
</html>)rawliteral";


void setupWifi() {
  if(state.getState() == State::NOT_CONFIGURED) {
    setupWifiAsAP();
  } else {
    setupWifiClient();
  }

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(getTemperature()).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(getHumidity()).c_str());
  });

  server.begin();
}

void setupWifiAsAP() {
  IPAddress local_IP(192,168,0,1);
  IPAddress gateway(192,168,0,1);
  IPAddress subnet(255,255,255,0);

  if(WiFi.softAPConfig(local_IP, gateway, subnet)) {
    state.setState(State::NOT_CONFIGURED);
    printMsg("APConfig set");
    delay(2000);
  } else {
    printMsg("Cannot set AP config");
    delay(2000);
    return;
  }

  if(WiFi.softAP("Dbox", "dddddddd")) {
    printMsg("AP configured.");
    delay(2000);
  } else {
    printMsg("Cannot start the AP");
    delay(2000);
  }
  wifi_status = WiFi.status();
}

void setupWifiClient() {
  WiFi.begin(state.getSSID(), state.getWifiPassword());

  while (wifi_status != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    wifi_status = WiFi.status();
  }
  
  wifi_status = WiFi.status();
}

