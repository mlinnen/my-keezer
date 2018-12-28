#include "mqtt.h"

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void mqtt_setup(Config &config)
{
  mqttClient.setServer(config.mqtt_server, config.mqtt_port);
  mqttClient.setCallback(mqtt_callback);
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  
}

void mqtt_reconnect(Config &config) {
  // is it time to try a re-connect to the MQTT broker?
  //if (_mqttReconnectTimer.isExpired() || _initialSetup)
  //{
    // Make sure we are not already connected
    if (!mqttClient.connected())
    {
      Serial.print("Attempting MQTT connection...");
      // Create a random client ID
      String clientId = "ESP8266Client-";
      clientId += String(random(0xffff), HEX);
      // Attempt to connect
      if (mqttClient.connect(clientId.c_str(),config.mqtt_user_name,config.mqtt_user_password)) {
        Serial.println("connected");
        // Once connected, re-subscribe

      } else {
        Serial.print("failed, rc=");
        Serial.print(mqttClient.state());
        Serial.println(" try again in 30 seconds");
        //_mqttReconnectTimer.restart();
      }
    }
  //}
}

void mqtt_subscribe() {

}

void mqtt_publish() {
    
    lightcontroller_publish(mqttClient);
    temperaturecontroller_publish(mqttClient);
}

void mqtt_loop(Config &config) {

}

