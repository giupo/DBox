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
<!doctype html>
<html lang="en">
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>DBOX</title>
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 2.0rem; }
    p { font-size: 1.5rem; }
    .units { font-size: 1.0rem; }
    .labels {
      font-size: 1.0rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }

    .temperature {
      color: rgb(113,135,254);
    }

    .humidity {
      color: rgb(99,152,143);
    }
    

  </style>
</head>
<body>
  <h2>DBOX Server</h2>
  <p>
    <span class="labels">Temperature</span> 
    <span class="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <span class="labels">Humidity</span>
    <span class="humidity">%HUMIDITY%</span>
    <sup class="units">%</sup>
  </p>
  <p class"labels">
    SSID <input type="text" name="ssid"/>
  </p>
  <p class=labels">
    Passkey <input type="text" name="passkey"/>
  </p>
  <p class="labels">
    <input type="button" class="button button4" onclick="updateWifi();" value="Apply" />
  </p>

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
    }, 3000 ) ;

    setInterval(function ( ) {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("humidity").innerHTML = this.responseText;
        }
      };
      xhttp.open("GET", "/humidity", true);
      xhttp.send();
    }, 3000 ) ;

  var updateWifi = function() {
    console.log("ciao mondo");
    alert("ciao mondo");
  };
</script>
</body>
</html>
)rawliteral";


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

  server.on("/updateWifi", HTTP_POST, [](AsyncWebServerRequest *request) {
    int n_params = request->params();
    String ssid = "";
    String pass = "";
    for(int i=0; i<n_params; i++){
      AsyncWebParameter* p = request->getParam(i);
      if(p->name() == "ssid") {
        ssid = p->value(); 
      } else if(p->name() == "passkey") {
        pass = p->value();
      }        
    }
    if(ssid != "" && pass != "") {
      state.saveWiFiCredentials(ssid, pass);
    }
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

