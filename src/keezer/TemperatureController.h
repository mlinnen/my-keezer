#ifndef TemperatureController_h
#define TemperatureController_h

#include "Arduino.h"
#include "TemperatureSensor.h"
#include <RBD_Button.h>


class TemperatureController
{
  public:
    TemperatureController(int relayPin, float lowSetpoint, float highSetpoint);
    void setup();
    boolean loop();
    float highSetPointTemperature();
    float averageSetPointTemperature();
    float lowSetPointTemperature();
    float topTemperature();
    float bottomTemperature();
    float averageTemperature();
    boolean compressor();
  private:
    int _relayPin;
    boolean _compressor;
    TemperatureSensor *_tempSensor;
    float _lowSetpoint = 0;
    float _highSetpoint = 0;
    float _averageCurrentTemp = 0;
};

#endif
