#include "mqtt.h"

WiFiClient espClient;
PubSubClient mqttClient(espClient);
RBD::Timer _mqttReconnectTimer;

void mqtt_setup(Config &config)
{
  mqttClient.setServer(config.mqtt_server, config.mqtt_port);
  mqttClient.setCallback(mqtt_callback);

  // Setup the timer to retry a connection to MQTT in 30 seconds
  _mqttReconnectTimer.setTimeout(30000);
}

void mqtt_callback(char* intopic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(intopic);
  Serial.print("] ");

  String topic = intopic;
  String message = "";

  for (int i = 0; i < length; i++) {
    message.concat((char)payload[i]);
  }
  Serial.println(message);

  mqtt_message_handler(topic, message);
  // TODO other callbacks for handling messages
}

void mqtt_reconnect(Config &config) {
  // is it time to try a re-connect to the MQTT broker?
  if (_mqttReconnectTimer.isExpired())
  {
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
        mqtt_subscribe();
      } else {
        Serial.print("failed, rc=");
        Serial.print(mqttClient.state());
        Serial.println(" try again in 30 seconds");
        _mqttReconnectTimer.restart();
      }
    }
  }
}

void mqtt_subscribe() {
    mqttClient.subscribe(MQTT_TOPIC_PING);
}

void mqtt_publish() {
    
    lightcontroller_publish(mqttClient);
    temperaturecontroller_publish(mqttClient);
}

void mqtt_loop(Config &config) {
    mqtt_reconnect(config);
    mqttClient.loop();
}

void mqtt_message_handler(String topic, String message) {
  if (topic.equalsIgnoreCase(MQTT_TOPIC_PING)){
    mqttClient.publish(MQTT_TOPIC_PINGR, " ");
  }
}

