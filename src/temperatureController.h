#ifndef TemperatureController_h
#define TemperatureController_h

#include <Arduino.h>
#include "temperatureSensor.h"
#include "keezerLCD.h"
#include <RBD_Button.h>
#include <RBD_Timer.h>
#include <PubSubClient.h>
#include "config.h"

void temperaturecontroller_setup(float lowSetpoint, float highSetpoint, int publishTemperatureSeconds);
boolean temperaturecontroller_loop(float fanTemperatureLow, float fanTemperatureHigh);
float temperaturecontroller_highSetPointTemperature();
float temperaturecontroller_averageSetPointTemperature();
float temperaturecontroller_lowSetPointTemperature();
boolean temperaturecontroller_compressor();
void temperaturecontroller_publish(PubSubClient &mqttClient);
void temperaturecontroller_publish(PubSubClient &mqttClient, const char* topic, float temp);
void temperaturecontroller_publish(PubSubClient &mqttClient, const char* topic, boolean value);


#endif
