#ifndef LightController_h
#define LightController_h

#include "Arduino.h"
#include <RBD_Timer.h>


class LightController
{
  public:
    LightController(int motionSensorPin,int lightRelayPin);
    void setup();
    boolean loop();
  private:
    int _lightRelayPin;
    int _motionSensorPin;
};

#endif
