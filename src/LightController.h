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
    void setup(int motionTimeOutSeconds, int lightOnSeconds);
    boolean loop();
  private:
    int _lightRelayPin;
    int _motionSensorPin;
    PubSubClient _client;
    void publish(const char* topic, boolean value);
    boolean _lastMotion;
    boolean _lastLight;
};

#endif
