#include "LightController.h"

RBD::Timer motionTimer;
RBD::Timer lightTimer;

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
    motionTimer.setTimeout(DEFAULT_MOTION_TIMEOUT_SECONDS * 1000);
    lightTimer.setTimeout(DEFAULT_LIGHT_ON_SECONDS * 1000);
}

boolean LightController::loop()
{
    if (digitalRead(_motionSensorPin)){
        // Turn on the lights and start the timers
        digitalWrite(_lightRelayPin,LOW);
        if (_lastMotion==false) {
            publish(MQTT_TOPIC_MOTION,true);
        }
        if (_lastLight==false) {
            publish(MQTT_TOPIC_LIGHT,true);
        }
        motionTimer.restart();
        lightTimer.restart();
        _lastMotion = true;
        _lastLight = true;
    }

    if(motionTimer.onExpired()) {
        publish(MQTT_TOPIC_MOTION,false);
        _lastMotion = false;
    }

    if(lightTimer.onExpired()) {
        // turn off the lights
        digitalWrite(_lightRelayPin,HIGH);
        publish(MQTT_TOPIC_LIGHT,false);
        _lastLight = false;
    }

    return true;
}

void LightController::publish(const char* topic, boolean value)
{
    if (_client.connected()) 
    {
      if (value) {
        _client.publish(topic, "1");
      }
      else {
        _client.publish(topic, "0");
      }
    }
}


