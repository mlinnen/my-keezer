#include "Arduino.h"
#include "TemperatureController.h"
#include "LightController.h"
#include "TemperatureLCD.h"
#include "ctype.h"
#include "config.h"
#include "mywifi.h"
#include <ESP8266WiFi.h>

TemperatureController *tempController;
LightController *lightController;

const char* ssid = WLAN_SSID;
const char* password = WLAN_PASS;

WiFiClient espClient;

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

void setup()
{
  Serial.begin(9600);

    // Delay a little bit so we can catch the serial port data during setup
  delay(5000);

  setup_wifi();
  
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
  // TODO Look for any messages that came in over MQTT

  // Process all the loops
  tempController->loop();
  lightController->loop();

  // TODO Determine if any state changes or sensor changes need to be broadcasted out over MQTT
}
