#ifndef TemperatureLCD_h
#define TemperatureLCD_h

#include "Arduino.h"
#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"
#include "TemperatureController.h"

class TemperatureLCD
{
  public:
    TemperatureLCD(TemperatureController *tempController);
    void setup();
    void loop();
};

#endif
