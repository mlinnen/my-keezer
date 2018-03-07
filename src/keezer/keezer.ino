#include "TemperatureController.h"
#include "TemperatureLCD.h"
#include "ctype.h"
#include "config.h"

TemperatureController *tempController;
TemperatureLCD *tempLCD;

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

  tempLCD = new TemperatureLCD(tempController);

  // Execute the setup on the temperature controller
  tempController->setup();
  tempLCD->setup();
}

void loop()
{
  // TODO Look for any messages that came in over MQTT

  // Process all the loops
  tempController->loop();
  tempLCD->loop();

  // TODO Determine if any state changes or sensor changes need to be broadcasted out over MQTT
}
