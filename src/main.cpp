#include "Arduino.h"
#include "TemperatureController.h"
#include "LightController.h"
#include "TemperatureLCD.h"
#include "ctype.h"
#include "config.h"
#include "mywifi.h"
#include "mymqttbroker.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

TemperatureController *tempController;
LightController *lightController;

const char* ssid = WLAN_SSID;
const char* password = WLAN_PASS;
const char* mqtt_server = MQTT_SERVER;

const char* mqttPingTopic = MQTT_TOPIC_PING;
const char* mqttPingResponseTopic = MQTT_TOPIC_PINGR;

RBD::Timer _mqttReconnectTimer;
RBD::Timer _wifiConnectTimer;

boolean _initialSetup = true;

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {

  if (!_initialSetup && !_wifiConnectTimer.isExpired())
  {
    return;
  }
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

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
  // is it time to try a re-connect?
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
      if (client.connect(clientId.c_str(),MQTT_USERNAME,MQTT_PASSWORD)) {
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

  // Delay a little bit so we can catch the serial port data during setup
  delay(5000);

  // Setup a timer to connect to Wifi for 30 seconds
  _wifiConnectTimer.setTimeout(30000);
  //_wifiConnectTimer.restart();

  setup_wifi();

  // Setup a timer to connect to MQTT after 30 seconds
  _mqttReconnectTimer.setTimeout(30000);
  _mqttReconnectTimer.restart();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  tempController = new TemperatureController(
    16, 12, 
    DEFAULT_TEMPERATURE_SETPOINT_LOW, 
    DEFAULT_TEMPERATURE_SETPOINT_HIGH,
    client);
  lightController = new LightController(14,15);

  tempController->setup();
  lightController->setup();
  
  reconnect();

  _initialSetup = false;

}

void loop()
{
  // Connect to Wifi if not connected
  if (WiFi.status() != WL_CONNECTED)
  {
    setup_wifi();
  }
  // Check connection to MQTT broker
 if (!client.connected()) {
    reconnect();
  }
  if (client.connected()) {client.loop();}

  // Process all the loops
  tempController->loop();
  lightController->loop();

  // TODO Determine if any state changes or sensor changes need to be broadcasted out over MQTT
}
