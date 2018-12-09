#ifndef TemperatureController_h
#define TemperatureController_h

#include "Arduino.h"
#include "TemperatureSensor.h"
#include <RBD_Button.h>
#include <RBD_Timer.h>
#include <PubSubClient.h>
#include "mymqttbroker.h"
#include "config.h"

class TemperatureController
{
  public:
    TemperatureController(int compressorRelayPin, int fanRelayPin, float lowSetpoint, float highSetpoint, PubSubClient &client);
    void setup();
    boolean loop();
    float highSetPointTemperature();
    float averageSetPointTemperature();
    float lowSetPointTemperature();
    float topTemperature();
    float bottomTemperature();
    float averageTemperature();
    boolean compressor();
  private:
    int _compressorRelayPin;
    int _fanRelayPin;
    boolean _compressor;
    boolean _lastCompressor;
    boolean _fan;
    boolean _lastFan;
    TemperatureSensor *_tempSensor;
    float _lowSetpoint = 0;
    float _highSetpoint = 0;
    float _averageCurrentTemp = 0;
    PubSubClient _client;
    RBD::Timer _publishTempTimer;
    void publish(const char* topic, float temp);
    void publish(const char* topic, boolean compressor);
};

#endif
