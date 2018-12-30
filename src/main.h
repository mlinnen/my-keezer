#ifndef MAIN_H
#define MAIN_H

#include "Arduino.h"
#include "config.h"
#include "keezerwifi.h"
#include "mqtt.h"
#include "jsonFile.h"
#include "keezerLCD.h"
#include "temperatureController.h"
#include "LightController.h"

Config config;

void setup();
void loop();

#endif