#ifndef TemperatureSensor_h
#define TemperatureSensor_h

#define TEMP_TOP_SENSOR_INDEX 2
#define TEMP_BOTTOM_SENSOR_INDEX 1
#define TEMP_EXTERIOR_SENSOR_INDEX 0

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
