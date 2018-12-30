#ifndef KeezerLCD_h
#define KeezerLCD_h

#include "Arduino.h"
#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"
#include "temperatureController.h"
#include "temperatureSensor.h"

void keezerlcd_setup();
void keezerlcd_changeMode();
void keezerlcd_changeMode(int mode);
void keezerlcd_print();
void keezerlcd_printText(int mode);
void keezerlcd_printVariables(int mode);

#endif
