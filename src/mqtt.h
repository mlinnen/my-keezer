#ifndef MQTT_H
#define MQTT_H

#include "Arduino.h"
#include <WiFiManager.h>
#include <PubSubClient.h>
#include "config.h" 
#include "LightController.h"
#include "temperatureController.h"

void mqtt_setup(Config &config);
void mqtt_loop(Config &config);
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void mqtt_reconnect(Config &config);
void mqtt_subscribe();
void mqtt_publish();

#endif
