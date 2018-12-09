#include "LightController.h"

RBD::Timer timer;

LightController::LightController(int motionSensorPin,int lightRelayPin, PubSubClient &client)
{
    _motionSensorPin = motionSensorPin;
    _lightRelayPin = lightRelayPin;
    _client = client;
}

void LightController::setup()
{
    pinMode(_lightRelayPin, OUTPUT);
    pinMode(_motionSensorPin, INPUT);
    digitalWrite(_lightRelayPin,HIGH);
}

boolean LightController::loop()
{
    if (digitalRead(_motionSensorPin)){
        // Turn on the lights and start the timer
        digitalWrite(_lightRelayPin,LOW);
        if (_lastMotion==false) {
            publishMotion(MQTT_TOPIC_MOTION,true);
        }
        timer.setTimeout(DEFAULT_MOTION_TIMEOUT_SECONDS);
        timer.restart();
        _lastMotion = true;
    }

    if(timer.onExpired()) {
        // turn off the lights
        digitalWrite(_lightRelayPin,HIGH);
        publishMotion(MQTT_TOPIC_MOTION,false);
        _lastMotion = false;
    }

    return true;
}

void LightController::publishMotion(const char* topic, boolean motion)
{
    if (_client.connected()) 
    {
      if (motion) {
        _client.publish(topic, "1");
      }
      else {
        _client.publish(topic, "0");
      }
    }
}


