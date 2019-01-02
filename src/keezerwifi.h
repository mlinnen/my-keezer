#ifndef KEEZER_WIFI_H
#define KEEZER_WIFI_H

#include "Arduino.h"
#include "config.h" 
#include "keezerLCD.h"
#include <WiFiManager.h>
#include <RBD_Timer.h>

boolean keezerwifi_saveConfig();
void keezerwifi_setup(Config &config, boolean manualConnect);
void keezerwifi_loop(Config &config);
void keezerwifi_saveConfigCallback();

#endif
