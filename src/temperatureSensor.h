#ifndef TemperatureSensor_h
#define TemperatureSensor_h

#include "Arduino.h"
#include <RBD_Timer.h>
#include <OneWire.h>
#include <DallasTemperature.h>

void temperaturesensor_setup(boolean celsius, int frequencySeconds);
boolean temperaturesensor_loop();
float temperaturesensor_topTemperature();
float temperaturesensor_averageTemperature();
float temperaturesensor_bottomTemperature();
float temperaturesensor_exteriorTemperature();


#endif
