#include "TemperatureController.h"
#include "ctype.h"
#include "config.h"

TemperatureController *tempController;

void setup()
{
  Serial.begin(9600);

  // Delay a little bit so we can catch the serial port data during setup
  delay(5000);

  // Initialize the temperature controller
  tempController = new TemperatureController(
    12, 
    DEFAULT_TEMPERATURE_SETPOINT_LOW, 
    DEFAULT_TEMPERATURE_SETPOINT_HIGH);

  // Execute the setup on the temperature controller
  tempController->setup();
}

void loop()
{
  // TODO Look for any messages that came in over MQTT

  // Process all the loops
  tempController->loop();

  // TODO Determine if any state changes or sensor changes need to be broadcasted out over MQTT
}
