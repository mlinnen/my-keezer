#include "TemperatureController.h"
#include "TemperatureSensor.h"
#include "TemperatureLCD.h"

TemperatureLCD *_tempLCD;
RBD::Button _modeButton(13);

TemperatureController::TemperatureController(int compressorRelayPin,int fanRelayPin, float lowSetpoint, float highSetpoint, PubSubClient &client)
{
  _lowSetpoint = lowSetpoint;
  _highSetpoint = highSetpoint;
  _compressorRelayPin = compressorRelayPin;
  _fanRelayPin = fanRelayPin;
  _tempLCD = new TemperatureLCD(this);
  _client = client;
}

void TemperatureController::setup(int publishTemperatureSeconds)
{
  // Perform any onetime setup.
  _tempSensor = new TemperatureSensor(false,1);
  _tempSensor->setup();

  _publishTempTimer.setTimeout(publishTemperatureSeconds * 1000);
  _publishTempTimer.restart();

  pinMode(_compressorRelayPin, OUTPUT);
  pinMode(_fanRelayPin, OUTPUT);
  digitalWrite(_fanRelayPin, HIGH);
  digitalWrite(_compressorRelayPin, HIGH);
  
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

boolean TemperatureController::loop(float fanTemperatureLow, float fanTemperatureHigh)
{
  boolean refreshLCD = false;
  // Was the mode button pressed and if so then increment the mode of the 
  if (_modeButton.onPressed())
  {
    _tempLCD->changeMode();
    refreshLCD = true;
  }

  // This is the service loop that is called from the main program and will update the state of this component.
  if (_tempSensor->loop() == true)
  {

    if (this->averageTemperature()<_lowSetpoint) {
      _compressor = false;
      digitalWrite(_compressorRelayPin,HIGH);
    }
    if (this->averageTemperature()>_highSetpoint) {
      _compressor = true;
      digitalWrite(_compressorRelayPin,LOW);
    }

    // Look at the difference between the top and bottom temperature sensor to determine when the fans should be on
    float tempDelta = abs(this->topTemperature() - this->bottomTemperature());
    if (tempDelta>fanTemperatureHigh) { 
      _fan = true; 
      digitalWrite(_fanRelayPin,LOW);
    }
    if (tempDelta<fanTemperatureLow) { 
      _fan=false; 
      digitalWrite(_fanRelayPin,HIGH);
    }

    if (_compressor != _lastCompressor)
    {
      publish(MQTT_TOPIC_COMPRESSOR, _compressor);
      if (_compressor) {Serial.println("Compressor On");}
      else {Serial.println("Compressor Off");}
    }
    _lastCompressor = _compressor;

    if (_fan != _lastFan)
    {
      publish(MQTT_TOPIC_FAN, _fan);
      if (_fan) {Serial.println("Fan On");}
      else {Serial.println("Fan Off");}
    }
    _lastFan = _fan;

    refreshLCD = true;
  }

  if (_publishTempTimer.isExpired())
  {
    publish(MQTT_TOPIC_TEMP_AVG, this->averageTemperature());
    publish(MQTT_TOPIC_TEMP_BOTTOM, this->bottomTemperature());
    publish(MQTT_TOPIC_TEMP_TOP, this->topTemperature());
    Serial.print("Hi  Setpoint ");
    Serial.println(this->highSetPointTemperature(), 1);
    Serial.print("Avg Setpoint ");
    Serial.println(this->averageSetPointTemperature(), 1);
    Serial.print("Low Setpoint ");
    Serial.println(this->lowSetPointTemperature(), 1);

    Serial.print("Avg Temp ");
    Serial.print(this->averageTemperature(), 1);
    Serial.println();
    Serial.print("Bot Temp ");
    Serial.print(this->bottomTemperature(), 1);
    Serial.println();
    Serial.print("Top Temp ");
    Serial.print(this->topTemperature(), 1);
    Serial.println();

    _publishTempTimer.restart();
  }

  if (refreshLCD) {_tempLCD->print();}

  return true;
}

void TemperatureController::publish(const char* topic, float temp)
{
    if (_client.connected()) 
    {
      String temp_str; 
      char message_buff[20];
      temp_str = String(temp);
      temp_str.toCharArray(message_buff, temp_str.length() + 1); 
      _client.publish(topic, message_buff, true);
    }
}

void TemperatureController::publish(const char* topic, boolean value)
{
    if (_client.connected()) 
    {
      if (value) {
        _client.publish(topic, "1", true);
      }
      else {
        _client.publish(topic, "0", true);
      }
    }
}
