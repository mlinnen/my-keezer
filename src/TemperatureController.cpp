#include "temperatureController.h"

#define COMPRESSOR_RELAY_PIN 12
#define FAN_RELAY_PIN 16

RBD::Button _modeButton(13);

boolean _compressor = false;
boolean _lastCompressor;
boolean _fan = false;
boolean _lastFan;
float _lowSetpoint = 0;
float _highSetpoint = 0;
boolean _publishTemperature = false;
boolean _publishCompressor = false;
boolean _publishFan = false;
RBD::Timer _publishTempTimer;
void publish(const char* topic, float temp);
void publish(const char* topic, boolean compressor);

void temperaturecontroller_setup(float lowSetpoint, float highSetpoint, int publishTemperatureSeconds)
{
  _lowSetpoint = lowSetpoint;
  _highSetpoint = highSetpoint;

  // Perform any onetime setup.
  temperaturesensor_setup(false,1);

  _publishTempTimer.setTimeout(publishTemperatureSeconds * 1000);
  _publishTempTimer.restart();

  pinMode(COMPRESSOR_RELAY_PIN, OUTPUT);
  pinMode(FAN_RELAY_PIN, OUTPUT);
  digitalWrite(FAN_RELAY_PIN, HIGH);
  digitalWrite(COMPRESSOR_RELAY_PIN, HIGH);
  
  temperaturelcd_setup();

}

boolean temperaturecontroller_compressor()
{
  return _compressor;
}

float temperaturecontroller_averageSetPointTemperature()
{
  return (_lowSetpoint + _highSetpoint)/2;
}

float temperaturecontroller_lowSetPointTemperature()
{
  return _lowSetpoint;
}

float temperaturecontroller_highSetPointTemperature()
{
  return _highSetpoint;
}

boolean temperaturecontroller_loop(float fanTemperatureLow, float fanTemperatureHigh)
{
  boolean refreshLCD = false;
  // Was the mode button pressed and if so then increment the mode of the 
  if (_modeButton.onPressed())
  {
    temperaturelcd_changeMode();
    refreshLCD = true;
  }

  // This is the service loop that is called from the main program and will update the state of this component.
  if (temperaturesensor_loop() == true)
  {

    if (temperaturesensor_averageTemperature()<_lowSetpoint) {
      _compressor = false;
      digitalWrite(COMPRESSOR_RELAY_PIN,HIGH);
    }
    if (temperaturesensor_averageTemperature()>_highSetpoint) {
      _compressor = true;
      digitalWrite(COMPRESSOR_RELAY_PIN,LOW);
    }

    // Look at the difference between the top and bottom temperature sensor to determine when the fans should be on
    float tempDelta = abs(temperaturesensor_topTemperature() - temperaturesensor_bottomTemperature());
    if (tempDelta>fanTemperatureHigh) { 
      _fan = true; 
      digitalWrite(FAN_RELAY_PIN,LOW);
    }
    if (tempDelta<fanTemperatureLow) { 
      _fan=false; 
      digitalWrite(FAN_RELAY_PIN,HIGH);
    }

    if (_compressor != _lastCompressor)
    {
      _publishCompressor = true;
      if (_compressor) {Serial.println("Compressor On");}
      else {Serial.println("Compressor Off");}
    }
    _lastCompressor = _compressor;

    if (_fan != _lastFan)
    {
      _publishFan = true;
      if (_fan) {Serial.println("Fan On");}
      else {Serial.println("Fan Off");}
    }
    _lastFan = _fan;

    refreshLCD = true;
  }

  if (_publishTempTimer.isExpired())
  {
    _publishTemperature = true;
    Serial.print("Hi  Setpoint ");
    Serial.println(temperaturecontroller_highSetPointTemperature(), 1);
    Serial.print("Avg Setpoint ");
    Serial.println(temperaturecontroller_averageSetPointTemperature(), 1);
    Serial.print("Low Setpoint ");
    Serial.println(temperaturecontroller_lowSetPointTemperature(), 1);

    Serial.print("Avg Temp ");
    Serial.print(temperaturesensor_averageTemperature(), 1);
    Serial.println();
    Serial.print("Bot Temp ");
    Serial.print(temperaturesensor_bottomTemperature(), 1);
    Serial.println();
    Serial.print("Top Temp ");
    Serial.print(temperaturesensor_topTemperature(), 1);
    Serial.println();

    _publishTempTimer.restart();
  }

  if (refreshLCD) {temperaturelcd_print();}

  return true;
}

void temperaturecontroller_publish(PubSubClient &mqttClient) {

  if (_publishCompressor) {
    temperaturecontroller_publish(mqttClient, MQTT_TOPIC_COMPRESSOR, _compressor);
    _publishCompressor = false;
  }

  if (_publishFan) {
    temperaturecontroller_publish(mqttClient, MQTT_TOPIC_FAN, _fan);
    _publishFan = false;
  }

  if (_publishTemperature) {
    temperaturecontroller_publish(mqttClient, MQTT_TOPIC_TEMP_AVG, temperaturesensor_averageTemperature());
    temperaturecontroller_publish(mqttClient, MQTT_TOPIC_TEMP_BOTTOM, temperaturesensor_bottomTemperature());
    temperaturecontroller_publish(mqttClient, MQTT_TOPIC_TEMP_TOP, temperaturesensor_topTemperature());
    _publishTemperature = false;
  }
}

void temperaturecontroller_publish(PubSubClient &mqttClient, const char* topic, float temp)
{
    if (mqttClient.connected()) 
    {
      String temp_str; 
      char message_buff[20];
      temp_str = String(temp);
      temp_str.toCharArray(message_buff, temp_str.length() + 1); 
      mqttClient.publish(topic, message_buff, true);
    }
}

void temperaturecontroller_publish(PubSubClient &mqttClient, const char* topic, boolean value)
{
    if (mqttClient.connected()) 
    {
      if (value) {
        mqttClient.publish(topic, "1", true);
      }
      else {
        mqttClient.publish(topic, "0", true);
      }
    }
}
