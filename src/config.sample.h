/************************* Temperature Controller *********************************/

#define DEFAULT_TEMPERATURE_SETPOINT_HIGH 40.0
#define DEFAULT_TEMPERATURE_SETPOINT_LOW  38.0
#define DEFAULT_PUBLISH_TEMPERATURE_SECONDS 15.0    // How often the temperature is published over MQTT
#define DEFAULT_MOTION_TIMEOUT_SECONDS 30.0         // The time it takes to reset the motion sensor
#define DEFAULT_LIGHT_ON_SECONDS 120.0              // The time it takes to turn off the lights after motion has stopped
#define DEFAULT_TEMPERATURE_FAN_SETPOINT_HIGH 10.0  // The differance between top and bottom temp sensors for fans to turn on
#define DEFAULT_TEMPERATURE_FAN_SETPOINT_LOW 8.0    // The differance between top and bottom temp sensors for fans to turn off
