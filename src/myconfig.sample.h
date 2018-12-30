/************************* Default Settings ****************************/
#define DEFAULT_TEMPERATURE_SETPOINT_HIGH 40.0
#define DEFAULT_TEMPERATURE_SETPOINT_LOW  38.0
#define DEFAULT_PUBLISH_TEMPERATURE_SECONDS 15.0    // How often the temperature is published over MQTT
#define DEFAULT_MOTION_TIMEOUT_SECONDS 30.0         // The time it takes to reset the motion sensor
#define DEFAULT_LIGHT_ON_SECONDS 120.0              // The time it takes to turn off the lights after motion has stopped
#define DEFAULT_TEMPERATURE_FAN_SETPOINT_HIGH 10.0  // The differance between top and bottom temp sensors for fans to turn on
#define DEFAULT_TEMPERATURE_FAN_SETPOINT_LOW 8.0    // The differance between top and bottom temp sensors for fans to turn off

/************************* Wifi Settings *******************************/
#define WIFI_AP_NAME  "mykeezer-ap"                   // The name of the access point when setting up the wifi


/************************* MQTT Topics *********************************/

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
