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

float TemperatureController::setTemperature()
{
  return (_lowSetpoint + _highSetpoint)/2;
}

float TemperatureController::topTemperature()
{
  return _tempSensor->topTemperature();
}

float TemperatureController::bottomTemperature()
{
  return _tempSensor->bottomTemperature();
}

float TemperatureController::averageTemperature()
{
  return _tempSensor->averageTemperature();
}

boolean TemperatureController::loop()
{
  // This is the service loop that is called from the main program and will update the state of this component.
  if (_tempSensor->loop() == true)
  {
    _averageCurrentTemp = _tempSensor->averageTemperature();
    Serial.print("Avg Temp ");
    Serial.print(_averageCurrentTemp, 1);
    Serial.println();
    Serial.print("Bottom Temp ");
    Serial.print(_tempSensor->bottomTemperature(), 1);
    Serial.println();
    Serial.print("Top Temp ");
    Serial.print(_tempSensor->topTemperature(), 1);
    Serial.println();

    if (_averageCurrentTemp<_lowSetpoint) {
      Serial.println("Compressor Off");
      digitalWrite(_relayPin,HIGH);
      }
    if (_averageCurrentTemp>_highSetpoint) {
      Serial.println("Compressor On");
      digitalWrite(_relayPin,LOW);
      }
  }
}
