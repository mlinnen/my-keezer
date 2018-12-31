#include "temperatureController.h"

#define COMPRESSOR_RELAY_PIN 12
#define FAN_RELAY_PIN 16
#define MODE_BUTTON_PIN 13

RBD::Button _modeButton(MODE_BUTTON_PIN);

int _mode;
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
unsigned long pressedTime;

void temperaturecontroller_setup(float lowSetpoint, float highSetpoint, int publishTemperatureSeconds)
{
  _mode = 0;
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

  // Capture when the mode button was first pressed
  if (_modeButton.onPressed())
  {
    pressedTime = millis();
  }
  if (_modeButton.onReleased())
  {
    unsigned long releaseTime;
    releaseTime = millis();
    // Determine if the mode button was pressed for a long time (>3 seconds)
    if ((releaseTime-pressedTime)/1000 > 3) {
      Serial.println("Mode button released long press");
      temperaturecontroller_changeMode(0);
    }
    else
    {
      Serial.println("Mode button released short press");
      temperaturecontroller_changeMode();
      refreshLCD = true;
    }
  }

  // Check if a new set of temp readings exist
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
  
  // if the keezer lights are on then turn on the LCD
  if (lightcontroller_light()) {
    keezerlcd_on();
  }
  else {
    keezerlcd_off();
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

  if (refreshLCD) {keezerlcd_print();}

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

int temperaturecontroller_mode()
{
  return _mode;
}

void temperaturecontroller_changeMode()
{
  _mode = _mode + 1;
  if (_mode>3) {_mode = 1;}
  Serial.print("Mode: ");
  Serial.println(_mode);
}

void temperaturecontroller_changeMode(int mode)
{
  _mode = mode;
  Serial.print("Mode: ");
  Serial.println(_mode);
}
