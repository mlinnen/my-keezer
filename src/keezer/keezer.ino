#include "TemperatureController.h"
#include "ctype.h"
TemperatureController *tempController;

void setup()
{
  Serial.begin(9600);

  tempController = new TemperatureController(12, 43.0, 46.0);

  tempController->setup();
}

void loop()
{
  tempController->loop();
}
