#ifndef TemperatureSensor_h
#define TemperatureSensor_h

#include "Arduino.h"
#include <RBD_Timer.h>
#include <OneWire.h>
#include <DallasTemperature.h>

class TemperatureSensor
{
  public:
    TemperatureSensor(boolean celsius, int frequencySeconds);
    void setup();
    boolean loop();
    float topTemperature();
    float averageTemperature();
    float bottomTemperature();

  private:
    RBD::Timer _frequencyTimer;
    int _frequencySeconds;
    boolean _celsius;
    float _topTempearture;
    float _bottomTempearture;
    void printAddress(DeviceAddress deviceAddress);
};

#endif
