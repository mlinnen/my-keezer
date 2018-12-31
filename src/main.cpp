#include "main.h"

void setup()
{
  Serial.begin(9600);

  randomSeed(micros());

  keezerlcd_setup();

  // Delay a little bit to give a developer time to connect the monitor to the serial port before running through the rest os the setup
  delay(5000);

 if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    
    jsonfile_load("/config.txt",config);
    jsonfile_print("/config.txt");

    keezerwifi_setup(config, false);

    mqtt_setup(config);

    jsonfile_save("/config.txt", config);

  } else {
    Serial.println("failed to mount FS");
  }

  // Run the setup on the temparature controller
  temperaturecontroller_setup(config.temperature_set_point_low,config.temperature_set_point_high,config.publish_temperature_seconds);
  
  // Run the setup on the light controller
  lightcontroller_setup(config.motion_timeout_seconds,config.light_on_seconds);
  
}

void loop()
{
  // Connect to Wifi if not connected
  keezerwifi_loop(config);

  // Process all the loops
  mqtt_loop(config);

  temperaturecontroller_loop(config.temperature_fan_set_point_low,config.temperature_fan_set_point_high);

  lightcontroller_loop();

  mqtt_publish();

  // TODO Determine if any state changes or sensor changes need to be broadcasted out over MQTT
}
