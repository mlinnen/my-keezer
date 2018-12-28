#include "main.h"

//RBD::Timer _mqttReconnectTimer;

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

  // Run the setup on the temparature controller
  temperaturecontroller_setup(config.temperature_set_point_low,config.temperature_set_point_high,config.publish_temperature_seconds);
  
  // Run the setup on the light controller
  lightcontroller_setup(config.motion_timeout_seconds,config.light_on_seconds);
  
  // The inital setup was executed so set the variable to false
  _initialSetup = false;

}

void loop()
{
  // Connect to Wifi if not connected
  wifi_reconnect(config);

  // Process all the loops
  mqtt_loop(config);

  temperaturecontroller_loop(config.temperature_fan_set_point_low,config.temperature_fan_set_point_high);

  lightcontroller_loop();

  mqtt_publish();

  // TODO Determine if any state changes or sensor changes need to be broadcasted out over MQTT
}
