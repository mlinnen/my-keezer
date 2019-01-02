#include "LightController.h"

RBD::Timer motionTimer;
RBD::Timer lightTimer;

boolean _motion = false;
boolean _light = false;
boolean _publishLight = false;
boolean _publishMotion = false;
boolean _lastMotion;
boolean _lastLight;
int _override = -1;

boolean lightcontroller_light()
{
    return _lastLight;
}

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
    // read the motion sensor
    boolean sensorMotion = digitalRead(MOTION_SENSOR_PIN);

    // If motion was detected then restart the timers
    if (sensorMotion) {
        motionTimer.restart();
        // The light timer only gets restarted if it is not overriden
        if (_override==-1) {
            lightTimer.restart();
        }
    }

    // The timer evens out the motion detection
    if (motionTimer.isExpired()) {
        _motion = false;
    }
    else {
        _motion = true;
    }

    // If the motion has changed then lets publish it
    if (_motion != _lastMotion) {
        _publishMotion = true;
        if (_motion) { Serial.println("Motion On"); }
        else { Serial.println("Motion Off"); }
    }

    // Determine if the lights should be on or off
    if (_override==-1) {
        // Not in override mode so the light turns on based on the light timer
        if (lightTimer.isExpired()) {
            _light = false;
        } 
        else {
            _light = true;
        }
    }
    else if (_override==1) {
        _light = true;
    }
    else if (_override==0) {
        _light = false;
    }
    
    // If the light state has changed then lets publish it and set the relay
    if (_light!=_lastLight) {
        _publishLight = true;
        if (_light) {
            digitalWrite(LIGHT_RELAY_PIN,LOW);
            Serial.println("Light On");
        }
        else {
            digitalWrite(LIGHT_RELAY_PIN,HIGH);
            Serial.println("Light Off");
        }
    }
    
    _lastMotion = _motion;
    _lastLight = _light;

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

void lightcontroller_message_handler(String topic, String message) {
  if (topic.equalsIgnoreCase(MQTT_TOPIC_LIGHT_SET)) {
    if (message.equalsIgnoreCase("1")) { _override = 1; }
    else if (message.equalsIgnoreCase("0")) { _override = 0; }
    else { _override = -1; }
  }
}




