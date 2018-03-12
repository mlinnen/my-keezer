#include "TemperatureSensor.h"

#define TEMPERATURE_PRECISION 9
OneWire _oneWire(2);
DallasTemperature _ds18b20(&_oneWire);
DeviceAddress _topThermometer, _bottomThermometer;

TemperatureSensor::TemperatureSensor(boolean celsius, int frequencySeconds)
{
  _celsius = celsius;
  _frequencySeconds = frequencySeconds;
  // Dont allow for a sample of less than 2 seconds
  if (frequencySeconds<2) {_frequencySeconds=2;}
  else {_frequencySeconds = frequencySeconds;}

}

void TemperatureSensor::printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

float TemperatureSensor::topTemperature()
{
  return _topTempearture;
}

float TemperatureSensor::bottomTemperature()
{
  return _bottomTempearture;
}

float TemperatureSensor::averageTemperature()
{
  float sum = _bottomTempearture + _topTempearture;
  if (sum==0) {return 0.0;}
  return sum/2;
}

void TemperatureSensor::setup()
{
  // Perform any onetime setup.

  _ds18b20.begin();
  Serial.print("Locating temperature devices...");
  Serial.print("Found ");
  Serial.print(_ds18b20.getDeviceCount(), DEC);
  Serial.println(" temperature devices.");

  if (!_ds18b20.getAddress(_bottomThermometer, 0)) Serial.println("Unable to find address for temperature Device 0");
  if (!_ds18b20.getAddress(_topThermometer, 1)) Serial.println("Unable to find address for temperature Device 1");

  // Must be called before search()
  //_oneWire.reset_search();
  // assigns the first address found to insideThermometer
  //if (!_oneWire.search(_topThermometer)) Serial.println("Unable to find address for Top Thermometer");
  // assigns the seconds address found to outsideThermometer
  //if (!_oneWire.search(_bottomThermometer)) Serial.println("Unable to find address for Bottom Thermometer");

  Serial.print("Temperature Device 0 Address: ");
  printAddress(_bottomThermometer);
  Serial.println();

  Serial.print("Temperature Device 1 Address: ");
  printAddress(_topThermometer);
  Serial.println();

  _ds18b20.setResolution(_bottomThermometer, TEMPERATURE_PRECISION);
  _ds18b20.setResolution(_topThermometer, TEMPERATURE_PRECISION);

  Serial.print("Temperature Device 0 Resolution: ");
  Serial.print(_ds18b20.getResolution(_bottomThermometer), DEC);
  Serial.println();

  Serial.print("Temperature Device 1 Resolution: ");
  Serial.print(_ds18b20.getResolution(_topThermometer), DEC);
  Serial.println();

  _frequencyTimer.setTimeout(_frequencySeconds * 1000);
  _frequencyTimer.restart();
}

boolean TemperatureSensor::loop()
{
  boolean newReadingAvailable = false;
  float temp;

  if (_frequencyTimer.isExpired())
  {
    _ds18b20.requestTemperatures();
    float tempC = _ds18b20.getTempC(_topThermometer);
    if (!_celsius) {
      _topTempearture = DallasTemperature::toFahrenheit(tempC);
    }
    else {
      _topTempearture = tempC;
    }

    tempC = _ds18b20.getTempC(_bottomThermometer);
    if (!_celsius) {
      _bottomTempearture = DallasTemperature::toFahrenheit(tempC);
    }
    else {
      _bottomTempearture = tempC;
    }

    _frequencyTimer.setTimeout(_frequencySeconds * 1000);
    _frequencyTimer.restart();
    newReadingAvailable = true;
  }

  return newReadingAvailable;
}

