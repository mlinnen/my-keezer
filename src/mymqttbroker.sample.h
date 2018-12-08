/************************* MQTT Setup *********************************/

#define MQTT_SERVER      "127.0.0.1"            // Set you MQTT brokers IP
#define AIO_SERVERPORT   1883                   // use 8883 for SSL
#define MQTT_USERNAME    "username"             // The user name for the MQTT broker
#define MQTT_PASSWORD    "password"             // The user name's password for the MQTT broker

#define MQTT_TOPIC_PING  "/home/keezer/ping"    // The topic that all keezer devices listens to
#define MQTT_TOPIC_PINGR "/home/keezer/pingr/1" // The topic that this devices publishes as a response to a ping request
                                                // The 1 at the end of the topic is the deviceId in case you have more than 1 keezer