#include "main.h"

//#include <RBD_Timer.h>

#include "TemperatureController.h"
#include "LightController.h"
#include "TemperatureLCD.h"

//#include "JsonFileConfig.h"

TemperatureController *tempController;
LightController *lightController;

//const char* mqttPingTopic = MQTT_TOPIC_PING;
//const char* mqttPingResponseTopic = MQTT_TOPIC_PINGR;

//RBD::Timer _mqttReconnectTimer;
//RBD::Timer _wifiConnectTimer;

boolean _initialSetup = true;

void setup()
{
  Serial.begin(9600);

  randomSeed(micros());

  // Delay a little bit to give a developer time to connect the monitor to the serial port before running through the rest os the setup
  delay(5000);

 if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    
    jsonfile_load("/config.txt",config);
    jsonfile_print("/config.txt");

    wifi_setup(config);

    mqtt_setup(config);

    jsonfile_save("/config.txt", config);

  } else {
    Serial.println("failed to mount FS");
  }

  // Setup the timer to retry a connection to Wifi in 30 seconds
  //_wifiConnectTimer.setTimeout(30000);

  // Setup the timer to retry a connection to MQTT in 30 seconds
  //_mqttReconnectTimer.setTimeout(30000);

/*
  tempController = new TemperatureController(
    12, 16, 
    config.temperature_set_point_low, 
    config.temperature_set_point_high,
    client);
  lightController = new LightController(14,15, client);
*/

  // Run the setup on the temparature controller
  tempController->setup(config.publish_temperature_seconds);

  // Run the setup on the light controller
  lightController->setup(int(config.motion_timeout_seconds),int(config.light_on_seconds));
  
  // The inital setup was executed so set the variable to false
  _initialSetup = false;

}

void loop()
{
  // Connect to Wifi if not connected
  wifi_reconnect(config);

  mqtt_loop(config);

  // Process all the loops
  // if we are connected to the broker then run it's loop
  //if (client.connected()) {client.loop();}
  tempController->loop(config.temperature_fan_set_point_low,config.temperature_fan_set_point_high);
  lightController->loop();

  // TODO Determine if any state changes or sensor changes need to be broadcasted out over MQTT
}
