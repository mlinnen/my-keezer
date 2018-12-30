
#include "jsonFile.h"

// Loads the configuration from a file
void jsonfile_load(const char *filename, Config &config) {
  
  // Open file for reading
  File file = SPIFFS.open(filename, "r");

  // Allocate the memory pool on the stack.
  // Don't forget to change the capacity to match your JSON document.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonBuffer<512> jsonBuffer;

  // Parse the root object
  JsonObject &root = jsonBuffer.parseObject(file);

  if (!root.success())
    Serial.println(F("Failed to read file, using default configuration"));

  // Copy values from the JsonObject to the Config
  config.mqtt_port = root["mport"] | 1883;
  config.publish_temperature_seconds = root["pubTempSecs"] | DEFAULT_PUBLISH_TEMPERATURE_SECONDS;
  config.temperature_set_point_low = root["tempLow"] | DEFAULT_TEMPERATURE_SETPOINT_LOW;
  config.temperature_set_point_high = root["tempHigh"] | DEFAULT_TEMPERATURE_SETPOINT_HIGH;
  config.motion_timeout_seconds = root["motionSecs"] | DEFAULT_MOTION_TIMEOUT_SECONDS;
  config.light_on_seconds = root["lightSecs"] | DEFAULT_LIGHT_ON_SECONDS;
  config.temperature_fan_set_point_low = root["fanTempLow"] | DEFAULT_TEMPERATURE_FAN_SETPOINT_LOW;
  config.temperature_fan_set_point_high = root["fanTempHigh"] | DEFAULT_TEMPERATURE_FAN_SETPOINT_HIGH;
  strlcpy(config.mqtt_server,                   // <- destination
          root["mserver"] | "",                 // <- source
          sizeof(config.mqtt_server));          // <- destination's capacity
  strlcpy(config.mqtt_user_name,                // <- destination
          root["musername"] | "",               // <- source
          sizeof(config.mqtt_user_name));       // <- destination's capacity
  strlcpy(config.mqtt_user_password,            // <- destination
          root["muserpwd"] | "",                // <- source
          sizeof(config.mqtt_user_password));   // <- destination's capacity
  strlcpy(config.wifi_ssid,                     // <- destination
          root["wssid"] | "",                   // <- source
          sizeof(config.wifi_ssid));            // <- destination's capacity
  strlcpy(config.wifi_password,                 // <- destination
          root["wpwd"] | "",                    // <- source
          sizeof(config.wifi_password));        // <- destination's capacity

  // Close the file (File's destructor doesn't close the file)
  file.close();
}

// Saves the configuration to a file
void jsonfile_save(const char *filename, const Config &config) {
  // Delete existing file, otherwise the configuration is appended to the file

  // Open file for writing
  File file = SPIFFS.open(filename, "w");
  if (!file) {
    Serial.println(F("Failed to create file"));
    return;
  }
  // Allocate the memory pool on the stack
  // Don't forget to change the capacity to match your JSON document.
  // Use https://arduinojson.org/assistant/ to compute the capacity.
  StaticJsonBuffer<256> jsonBuffer;

  // Parse the root object
  JsonObject &root = jsonBuffer.createObject();

  // Set the values
  root["mserver"] = config.mqtt_server;
  root["mport"] = config.mqtt_port;
  root["musername"] = config.mqtt_user_name;
  root["muserpwd"] = config.mqtt_user_password;
  root["wssid"] = config.wifi_ssid;
  root["wpwd"] = config.wifi_password;
  root["pubTempSecs"] = config.publish_temperature_seconds;
  root["tempLow"] = config.temperature_set_point_low;
  root["tempHigh"] = config.temperature_set_point_high;
  root["lightSecs"] = config.light_on_seconds;
  root["fanTempLow"] = config.temperature_fan_set_point_low;
  root["fanTempHigh"] = config.temperature_fan_set_point_high;

  // Serialize JSON to file
  if (root.printTo(file) == 0) {
    Serial.println(F("Failed to write to file"));
  }

  // Close the file (File's destructor doesn't close the file)
  file.close();
}

// Prints the content of a file to the Serial
void jsonfile_print(const char *filename) {
  // Open file for reading
  File file = SPIFFS.open(filename, "r");
  if (!file) {
    Serial.println(F("Failed to read file"));
    return;
  }

  // Extract each characters by one by one
  while (file.available()) {
    Serial.print((char)file.read());
  }
  Serial.println();

  // Close the file (File's destructor doesn't close the file)
  file.close();
}

