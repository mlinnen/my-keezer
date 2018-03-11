#include "LightController.h"

RBD::Timer timer;

LightController::LightController(int motionSensorPin,int lightRelayPin)
{
    _motionSensorPin = motionSensorPin;
    _lightRelayPin = lightRelayPin;
}

void LightController::setup()
{
    pinMode(_lightRelayPin, OUTPUT);
    digitalWrite(_lightRelayPin,LOW);
}

boolean LightController::loop()
{
    if (digitalRead(_motionSensorPin)){
        // Turn on the lights and start the timer
        digitalWrite(_lightRelayPin,HIGH);
        timer.setTimeout(120000);
    }

    if(timer.onExpired()) {
        // turn off the lights
        digitalWrite(_lightRelayPin,LOW);
    }
}

