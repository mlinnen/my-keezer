#include "TemperatureController.h"
#include "ctype.h"
TemperatureController *tempController;

void setup()
{
  Serial.begin(9600);

  // Delay a little bit so we can catch the serial port data during setup
  delay(5000);

  // Initialize the temperature controller
  tempController = new TemperatureController(12, 40.0, 42.0);

  // Execute the setup on the temperature controller
  tempController->setup();
}

void loop()
{
  tempController->loop();
}
