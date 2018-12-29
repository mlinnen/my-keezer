#ifndef LightController_h
#define LightController_h

#include "Arduino.h"
#include <RBD_Timer.h>
#include <PubSubClient.h>
#include "config.h"

#define MOTION_SENSOR_PIN 14
#define LIGHT_RELAY_PIN 15

void lightcontroller_setup(int motionTimeOutSeconds, int lightOnSeconds);
boolean lightcontroller_loop();
void lightcontroller_publish(PubSubClient &mqttClient);

#endif
