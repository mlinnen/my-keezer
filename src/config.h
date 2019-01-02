#ifndef Config_h
#define Config_h

#include "myconfig.h"

struct Config {
  char mqtt_server[64];
  int mqtt_port;
  char mqtt_user_name[20];
  char mqtt_user_password[20];
  char wifi_ssid[20];
  char wifi_password[20];
  float temperature_fan_set_point_high;
  float temperature_fan_set_point_low;
  float temperature_set_point_high;
  float temperature_set_point_low;
  int publish_temperature_seconds;
  int motion_timeout_seconds;
  int light_on_seconds;
};

#endif