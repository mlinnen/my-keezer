#ifndef TemperatureLCD_h
#define TemperatureLCD_h

#include "Arduino.h"
#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"
#include "TemperatureController.h"
#include "TemperatureSensor.h"

void temperaturelcd_setup();
void temperaturelcd_changeMode();
void temperaturelcd_print();
void temperaturelcd_printText(int mode);
void temperaturelcd_printVariables(int mode);

#endif
