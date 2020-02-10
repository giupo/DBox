#ifndef DBOX_STATE
#define DBOX_STATE

#include "EEPROM.h"
#include "dbox_display.hpp"
#include "string.h"
#define EEPROM_ADDR 0



class State {
public:
  enum DBOX_state {
    NOT_CONFIGURED,
    CONFIGURED
  } dbox_state;

  State();
  virtual ~State() {}

  static void setup();

  void init();
  void save();

  String get_message();
  void saveWiFiCredentials(const String ssid, const String pwd);
  DBOX_state getState();
  void setState(DBOX_state state);

  String getSSID();
  String getWifiPassword();

  void print();
  void update();

private:
  String message;
  struct {
    char wifi_ssid[32] = "DBOX";
    char wifi_pwd[32] = "dbox";
    DBOX_state state;
  } data;
};

void setupState();

#endif