#ifndef TemperatureController_h
#define TemperatureController_h

#include "Arduino.h"
#include "TemperatureSensor.h"


class TemperatureController
{
  public:
    TemperatureController(int relayPin, float lowSetpoint, float highSetpoint);
    void setup();
    boolean loop();
  private:
    int _relayPin;
    TemperatureSensor *_tempSensor;
    float _lowSetpoint = 0;
    float _highSetpoint = 0;
    float _averageCurrentTemp = 0;
};

#endif
