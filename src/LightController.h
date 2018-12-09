#ifndef LightController_h
#define LightController_h

#include "Arduino.h"
#include <RBD_Timer.h>
#include <PubSubClient.h>
#include "mymqttbroker.h"
#include "config.h"

class LightController
{
  public:
    LightController(int motionSensorPin,int lightRelayPin, PubSubClient &client);
    void setup();
    boolean loop();
  private:
    int _lightRelayPin;
    int _motionSensorPin;
    PubSubClient _client;
    void publishMotion(const char* topic, boolean motion);
    boolean _lastMotion;
};

#endif
