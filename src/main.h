#ifndef MAIN_H
#define MAIN_H

#include "Arduino.h"
#include "config.h"
#include "wifi.h"
#include "mqtt.h"
#include "jsonFile.h"
#include "TemperatureController.h"
#include "LightController.h"

Config config;

void setup();
void loop();

#endif