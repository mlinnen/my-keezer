#ifndef TemperatureController_h
#define TemperatureController_h

#include <Arduino.h>
#include "temperatureSensor.h"
#include "LightController.h"
#include "keezerLCD.h"
#include <RBD_Button.h>
#include <RBD_Timer.h>
#include <PubSubClient.h>
#include "config.h"

void temperaturecontroller_setup(float lowSetpoint, float highSetpoint, int publishTemperatureSeconds);
boolean temperaturecontroller_loop(float fanTemperatureLow, float fanTemperatureHigh);
void temperaturecontroller_changeMode();
void temperaturecontroller_changeMode(int mode);
int temperaturecontroller_mode();
float temperaturecontroller_highSetPointTemperature();
float temperaturecontroller_averageSetPointTemperature();
float temperaturecontroller_lowSetPointTemperature();
boolean temperaturecontroller_compressor();
void temperaturecontroller_publish(PubSubClient &mqttClient);
void temperaturecontroller_publish(PubSubClient &mqttClient, const char* topic, float temp);
void temperaturecontroller_publish(PubSubClient &mqttClient, const char* topic, boolean value);


#endif
