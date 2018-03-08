#include "TemperatureController.h"
#include "TemperatureSensor.h"

TemperatureLCD *_tempLCD;
RBD::Button _modeButton(13);


TemperatureController::TemperatureController(int relayPin,float lowSetpoint, float highSetpoint)
{
  _lowSetpoint = lowSetpoint;
  _highSetpoint = highSetpoint;
  _relayPin = relayPin;
  _tempLCD = new TemperatureLCD(tempController);

}

void TemperatureController::setup()
{
  // Perform any onetime setup.
  _tempSensor = new TemperatureSensor(false,3);
  _tempSensor->setup();

  pinMode(_relayPin, OUTPUT);

  _tempLCD->setup();

}

boolean TemperatureController::compressor()
{
  return _compressor;
}

float TemperatureController::averageSetPointTemperature()
{
  return (_lowSetpoint + _highSetpoint)/2;
}

float TemperatureController::lowSetPointTemperature()
{
  return _lowSetpoint;
}

float TemperatureController::highSetPointTemperature()
{
  return _highSetpoint;
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
  // Was the mode button pressed and if so then increment the mode of the 
  if (_modeButton.onPressed())
  {
    _tempLCD->changeMode();
  }

  // This is the service loop that is called from the main program and will update the state of this component.
  if (_tempSensor->loop() == true)
  {
    _averageCurrentTemp = _tempSensor->averageTemperature();
    Serial.print("Hi  Setpoint ");
    Serial.println(highSetPointTemperature(), 1);
    Serial.print("Avg Setpoint ");
    Serial.println(averageSetPointTemperature(), 1);
    Serial.print("Low Setpoint ");
    Serial.println(lowSetPointTemperature(), 1);

    Serial.print("Avg Temp ");
    Serial.print(_averageCurrentTemp, 1);
    Serial.println();
    Serial.print("Bot Temp ");
    Serial.print(_tempSensor->bottomTemperature(), 1);
    Serial.println();
    Serial.print("Top Temp ");
    Serial.print(_tempSensor->topTemperature(), 1);
    Serial.println();

    if (_averageCurrentTemp<_lowSetpoint) {
      _compressor = false;
      digitalWrite(_relayPin,LOW);
      }
    if (_averageCurrentTemp>_highSetpoint) {
      _compressor = true;
      digitalWrite(_relayPin,HIGH);
      }

    if (_compressor) {Serial.println("Compressor On");}
    else {Serial.println("Compressor Off");}

    //_tempLCD->print();

  }
}
