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

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
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
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(),MQTT_USERNAME,MQTT_PASSWORD)) {
      Serial.println("connected");
      // Once connected, re-subscribe
      client.subscribe(mqttPingTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(9600);

    // Delay a little bit so we can catch the serial port data during setup
  delay(5000);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  tempController = new TemperatureController(
    16, 12, 
    DEFAULT_TEMPERATURE_SETPOINT_LOW, 
    DEFAULT_TEMPERATURE_SETPOINT_HIGH);
  lightController = new LightController(14,15);

  tempController->setup();
  lightController->setup();
}

void loop()
{
  // Connect to MQTT broker
 if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Process all the loops
  tempController->loop();
  lightController->loop();

  // TODO Determine if any state changes or sensor changes need to be broadcasted out over MQTT
}
