#ifndef WIFI_H
#define WIFI_H

#include "config.h" 
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

void wifi_setup(Config &config);
void wifi_reconnect(Config &config);


#endif
