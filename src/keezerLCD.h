#ifndef KeezerLCD_h
#define KeezerLCD_h

#include "Arduino.h"
#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"
#include "temperatureController.h"
#include "temperatureSensor.h"

void keezerlcd_setup();
void keezerlcd_print();
void keezerlcd_printText(int mode);
void keezerlcd_printVariables(int mode);
void keezerlcd_on();
void keezerlcd_off();

#endif
