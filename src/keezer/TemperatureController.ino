#include "TemperatureController.h"
#include "TemperatureSensor.h"



TemperatureController::TemperatureController(int relayPin,float lowSetpoint, float highSetpoint)
{
  _lowSetpoint = lowSetpoint;
  _highSetpoint = highSetpoint;
  _relayPin = relayPin;
}

void TemperatureController::setup()
{
  // Perform any onetime setup.
  _tempSensor = new TemperatureSensor(false,3);
  _tempSensor->setup();

  pinMode(_relayPin, OUTPUT);

}

boolean TemperatureController::loop()
{
  // This is the service loop that is called from the main program and will update the state of this component.
  if (_tempSensor->loop() == true)
  {
    _averageCurrentTemp = (_tempSensor->topTemperature() + _tempSensor->bottomTemperature()) / 2;
    Serial.print("Avg Temp ");
    Serial.print(_averageCurrentTemp, DEC);
    Serial.println();

    if (_averageCurrentTemp<_lowSetpoint) {Serial.println("Compressor Off");}
    if (_averageCurrentTemp>_highSetpoint) {Serial.println("Compressor On");}
  }
}
