#include "Arduino.h"
#include "TemperatureController.h"
#include "LightController.h"
#include "TemperatureLCD.h"
#include "ctype.h"
#include "config.h"

TemperatureController *tempController;
LightController *lightController;

void setup()
{
  Serial.begin(9600);

  // Delay a little bit so we can catch the serial port data during setup
  delay(5000);

  tempController = new TemperatureController(
    16, 12, 
    DEFAULT_TEMPERATURE_SETPOINT_LOW, 
    DEFAULT_TEMPERATURE_SETPOINT_HIGH);
  lightController = new LightController(14,15);

  tempController->setup();
  lightController->setup();
}

void loop()
{
  // TODO Look for any messages that came in over MQTT

  // Process all the loops
  tempController->loop();
  lightController->loop();

  // TODO Determine if any state changes or sensor changes need to be broadcasted out over MQTT
}
