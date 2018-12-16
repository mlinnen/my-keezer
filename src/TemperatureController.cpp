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

void TemperatureController::setup()
{
  // Perform any onetime setup.
  _tempSensor = new TemperatureSensor(false,1);
  _tempSensor->setup();

  _publishTempTimer.setTimeout(DEFAULT_PUBLISH_TEMPERATURE_SECONDS * 1000);
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

boolean TemperatureController::loop()
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

    _averageCurrentTemp = _tempSensor->averageTemperature();
    Serial.print("Hi  Setpoint ");
    Serial.println(highSetPointTemperature(), 1);
    Serial.print("Avg Setpoint ");
    Serial.println(averageSetPointTemperature(), 1);
    Serial.print("Low Setpoint ");
    Serial.println(lowSetPointTemperature(), 1);

    Serial.print("Avg Temp ");
    Serial.print(_averageCurrentTemp, 1);
    publish(MQTT_TOPIC_TEMP_AVG, _averageCurrentTemp);
    Serial.println();
    Serial.print("Bot Temp ");
    Serial.print(_tempSensor->bottomTemperature(), 1);
    publish(MQTT_TOPIC_TEMP_BOTTOM, _tempSensor->bottomTemperature());
    Serial.println();
    Serial.print("Top Temp ");
    Serial.print(_tempSensor->topTemperature(), 1);
    publish(MQTT_TOPIC_TEMP_TOP, _tempSensor->topTemperature());
    Serial.println();

    if (_averageCurrentTemp<_lowSetpoint) {
      _compressor = false;
      digitalWrite(_compressorRelayPin,HIGH);

      _fan = false;
      digitalWrite(_fanRelayPin,HIGH);
    }
    if (_averageCurrentTemp>_highSetpoint) {
      _compressor = true;
      digitalWrite(_compressorRelayPin,LOW);

      _fan = true;
      digitalWrite(_fanRelayPin,LOW);
    }

    if (_compressor) {Serial.println("Compressor On");}
    else {Serial.println("Compressor Off");}

    if (_fan) {Serial.println("Fan On");}
    else {Serial.println("Fan Off");}

    refreshLCD = true;
  }

  if (_publishTempTimer.isExpired())
  {
    publish(MQTT_TOPIC_TEMP_AVG, _averageCurrentTemp);
    publish(MQTT_TOPIC_TEMP_BOTTOM, _tempSensor->bottomTemperature());
    publish(MQTT_TOPIC_TEMP_TOP, _tempSensor->topTemperature());
    _publishTempTimer.restart();
  }

  if (_compressor != _lastCompressor)
  {
    publish(MQTT_TOPIC_COMPRESSOR, _compressor);
  }
  _lastCompressor = _compressor;

  if (_fan != _lastFan)
  {
    publish(MQTT_TOPIC_FAN, _fan);
  }
  _lastFan = _fan;

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
