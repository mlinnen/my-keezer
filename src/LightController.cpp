#include "LightController.h"

RBD::Timer motionTimer;
RBD::Timer lightTimer;

boolean _publishLight = false;
boolean _publishMotion = false;
boolean _lastMotion;
boolean _lastLight;

void lightcontroller_setup(int motionTimeOutSeconds, int lightOnSeconds)
{
    pinMode(LIGHT_RELAY_PIN, OUTPUT);
    pinMode(MOTION_SENSOR_PIN, INPUT);
    digitalWrite(LIGHT_RELAY_PIN,HIGH);
    motionTimer.setTimeout(motionTimeOutSeconds * 1000);
    lightTimer.setTimeout(lightOnSeconds * 1000);
}

boolean lightcontroller_loop()
{
    if (digitalRead(MOTION_SENSOR_PIN)){
        // Turn on the lights and start the timers
        digitalWrite(LIGHT_RELAY_PIN,LOW);
        if (_lastMotion==false) {
            _publishMotion = true;
            Serial.println("Motion On");
        }
        if (_lastLight==false) {
            _publishLight = true;
            Serial.println("Light On");
        }
        motionTimer.restart();
        lightTimer.restart();
        _lastMotion = true;
        _lastLight = true;
    }

    if(motionTimer.onExpired()) {
        _publishMotion = true;
        Serial.println("Motion Off");
        _lastMotion = false;
    }

    if(lightTimer.onExpired()) {
        // turn off the lights
        digitalWrite(LIGHT_RELAY_PIN,HIGH);
        _publishLight = true;
        Serial.println("Light Off");
        _lastLight = false;
    }

    return true;
}

void lightcontroller_publish(PubSubClient &mqttClient)
{
    if (mqttClient.connected()) 
    {
        if (_publishLight)
        {
            if (_lastLight)
                mqttClient.publish(MQTT_TOPIC_LIGHT,"1",true);
            else
                mqttClient.publish(MQTT_TOPIC_LIGHT,"0",true);
            _publishLight = false;
        }
        if (_publishMotion)
        {
            if (_lastMotion)
                mqttClient.publish(MQTT_TOPIC_MOTION,"1",true);
            else
                mqttClient.publish(MQTT_TOPIC_MOTION,"0",true);
            _publishMotion = false;
        }
    }
}

void lightcontroller_subscribe(PubSubClient &mqttClient)
{
    if (mqttClient.connected()) 
    {
        mqttClient.subscribe(MQTT_TOPIC_LIGHT);
    }
}



