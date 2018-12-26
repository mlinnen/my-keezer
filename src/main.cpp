#include <FS.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <PubSubClient.h>

#include <RBD_Timer.h>

#include "TemperatureController.h"
#include "LightController.h"
#include "TemperatureLCD.h"
#include "ctype.h"
//#include "mywifi.h"
//#include "mymqttbroker.h"

#include "JsonFileConfig.h"

TemperatureController *tempController;
LightController *lightController;

const char* mqttPingTopic = MQTT_TOPIC_PING;
const char* mqttPingResponseTopic = MQTT_TOPIC_PINGR;

RBD::Timer _mqttReconnectTimer;
RBD::Timer _wifiConnectTimer;

boolean _initialSetup = true;

WiFiClient espClient;
PubSubClient client(espClient);
const char *filename = "/config.txt";  // <- SD library uses 8.3 filenames
Config config;                         // <- global configuration object

void setup_wifi() {

  if (!_initialSetup && !_wifiConnectTimer.isExpired())
  {
    return;
  }
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  //Serial.println(ssid);

  //WiFi.begin(ssid, password);

  int numberOfTries = 10;
  if (!_initialSetup)
  {
    numberOfTries = 1;
  }
  int count = 0;
  while (count < numberOfTries) {
    if (WiFi.status() == WL_CONNECTED){ break;}
    delay(500);
    Serial.print(".");
    count++;
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("Unable to connect to WiFi but will try again later");
    _wifiConnectTimer.restart();
  }

}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  
  // If we get a ping request then respond to it
  if (strstr(topic,mqttPingTopic)){
    client.publish(mqttPingResponseTopic, " ");
  }
}

void reconnect() {
  // is it time to try a re-connect to the MQTT broker?
  if (_mqttReconnectTimer.isExpired() || _initialSetup)
  {
    // Make sure we are not already connected
    if (!client.connected())
    {
      Serial.print("Attempting MQTT connection...");
      // Create a random client ID
      String clientId = "ESP8266Client-";
      clientId += String(random(0xffff), HEX);
      // Attempt to connect
      if (client.connect(clientId.c_str(),config.mqtt_user_name,config.mqtt_user_password)) {
        Serial.println("connected");
        // Once connected, re-subscribe
        client.subscribe(mqttPingTopic);
        // TODO add any Other Subscriptions
      } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 30 seconds");
        _mqttReconnectTimer.restart();
      }
    }
  }
}

void setup()
{
  Serial.begin(9600);

  randomSeed(micros());

  // Delay a little bit to give a developer time to connect the monitor to the serial port before running through the rest os the setup
  delay(5000);

 if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    JsonFileConfig *configFile;
    configFile = new JsonFileConfig();
    configFile->loadConfiguration(filename, config);
    configFile->printFile(filename);

    WiFiManager wifiManager;
    //reset saved settings
    //wifiManager.resetSettings(); // For testing

    // Setup some custom parameters to capture more details needed for connecting the keezer to MQTT
    char portstr[5];
    String str;
    str = String(config.mqtt_port);
    str.toCharArray(portstr,5);
    WiFiManagerParameter custom_mqtt_server("server", "MQTT Server", config.mqtt_server, 64);
    WiFiManagerParameter custom_mqtt_port("port", "MQTT Port", portstr, 5);
    WiFiManagerParameter custom_mqtt_user_name("username", "MQTT User Name", config.mqtt_user_name, 20);
    WiFiManagerParameter custom_mqtt_user_password("userpassword", "MQTT User Password", config.mqtt_user_password, 20);
    wifiManager.addParameter(&custom_mqtt_server);
    wifiManager.addParameter(&custom_mqtt_port);
    wifiManager.addParameter(&custom_mqtt_user_name);
    wifiManager.addParameter(&custom_mqtt_user_password);
    wifiManager.setConfigPortalTimeout(120);
    if (!wifiManager.autoConnect("keezer-ap")){
      Serial.println("Failed to connect to WiFi");
    }

    WiFi.SSID().toCharArray(config.wifi_ssid,20);
    WiFi.psk().toCharArray(config.wifi_password,20);
    std::copy(custom_mqtt_server.getValue(),custom_mqtt_server.getValue() + strlen(custom_mqtt_server.getValue()),config.mqtt_server);
    std::copy(custom_mqtt_user_name.getValue(),custom_mqtt_user_name.getValue() + strlen(custom_mqtt_user_name.getValue()),config.mqtt_user_name);
    std::copy(custom_mqtt_user_password.getValue(),custom_mqtt_user_password.getValue() + strlen(custom_mqtt_user_password.getValue() ),config.mqtt_user_password);

    configFile->saveConfiguration(filename, config);
  } else {
    Serial.println("failed to mount FS");
  }

  // Setup the timer to retry a connection to Wifi in 30 seconds
  _wifiConnectTimer.setTimeout(30000);

  // Try to connect to the wifi
  setup_wifi();

  // Setup the timer to retry a connection to MQTT in 30 seconds
  _mqttReconnectTimer.setTimeout(30000);

  client.setServer(config.mqtt_server, config.mqtt_port);
  client.setCallback(callback);

  tempController = new TemperatureController(
    12, 16, 
    config.temperature_set_point_low, 
    config.temperature_set_point_high,
    client);
  lightController = new LightController(14,15, client);

  // Run the setup on the temparature controller
  tempController->setup(config.publish_temperature_seconds);

  // Run the setup on the light controller
  lightController->setup(int(config.motion_timeout_seconds),int(config.light_on_seconds));
  
  // Try to connect to the MQTT broker
  reconnect();

  // The inital setup was executed so set the variable to false
  _initialSetup = false;

}

void loop()
{
  // Connect to Wifi if not connected
  if (WiFi.status() != WL_CONNECTED) {
    // Try to connect to wifi
    setup_wifi();
  }

  // Connect to MQTT broker if not connected
  if (!client.connected()) {
    // Try to connect to MQTT broker
    reconnect();
  }

  // Process all the loops
  // if we are connected to the broker then run it's loop
  if (client.connected()) {client.loop();}
  tempController->loop(config.temperature_fan_set_point_low,config.temperature_fan_set_point_high);
  lightController->loop();

  // TODO Determine if any state changes or sensor changes need to be broadcasted out over MQTT
}
