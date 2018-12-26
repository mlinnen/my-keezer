/************************* MQTT Setup *********************************/

#define MQTT_TOPIC_PING  "/home/keezer/ping"    // The topic that all keezer devices listens to
#define MQTT_TOPIC_PINGR "/home/keezer/pingr/1" // The topic that this devices publishes as a response to a ping request
                                                // The 1 at the end of the topic is the deviceId in case you have more than 1 keezer

#define MQTT_TOPIC_TEMP_AVG     "/home/keezer/1/temp/avg"    // The topic for the keezer average temperature reading
#define MQTT_TOPIC_TEMP_TOP     "/home/keezer/1/temp/top"    // The topic for the keezer top temperature sensor reading
#define MQTT_TOPIC_TEMP_BOTTOM  "/home/keezer/1/temp/bottom" // The topic for the keezer bottom temperature sensor reading

#define MQTT_TOPIC_MOTION       "/home/keezer/1/motion/value"       // The topic for the keezer motion sensor
#define MQTT_TOPIC_COMPRESSOR   "/home/keezer/1/compressor/value"   // The topic for the keezer compressor
#define MQTT_TOPIC_FAN          "/home/keezer/1/fan/value"          // The topic for the keezer fan
#define MQTT_TOPIC_LIGHT        "/home/keezer/1/light/value"        // The topic for the keezer light
