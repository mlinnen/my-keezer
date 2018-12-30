#ifndef KEEZER_WIFI_H
#define KEEZER_WIFI_H

#include "config.h" 
#include "keezerLCD.h"
#include <WiFiManager.h>
#include <RBD_Timer.h>

void keezerwifi_setup(Config &config);
void keezerwifi_reconnect(Config &config);


#endif
