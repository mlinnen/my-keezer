#include "keezerwifi.h"

RBD::Timer _wifiConnectTimer;


void keezerwifi_setup(Config &config) {

    _wifiConnectTimer.setTimeout(30000);

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
    keezerlcd_changeMode(0);
    keezerlcd_print();
    if (!wifiManager.autoConnect(WIFI_AP_NAME)){
      Serial.println("Failed to connect to WiFi");
    }

    WiFi.SSID().toCharArray(config.wifi_ssid,20);
    WiFi.psk().toCharArray(config.wifi_password,20);
    std::copy(custom_mqtt_server.getValue(),custom_mqtt_server.getValue() + strlen(custom_mqtt_server.getValue()),config.mqtt_server);
    std::copy(custom_mqtt_user_name.getValue(),custom_mqtt_user_name.getValue() + strlen(custom_mqtt_user_name.getValue()),config.mqtt_user_name);
    std::copy(custom_mqtt_user_password.getValue(),custom_mqtt_user_password.getValue() + strlen(custom_mqtt_user_password.getValue() ),config.mqtt_user_password);

}

void keezerwifi_reconnect(Config &config) {
  if (WiFi.status() == WL_CONNECTED)
  {
    return;
  }
  if (!_wifiConnectTimer.isExpired())
  {
    return;
  }
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(config.wifi_ssid);

  WiFi.begin(config.wifi_ssid, config.wifi_password);

  int count = 0;
  while (count < 10) {
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
    Serial.println("Unable to connect to WiFi but will try again in 30 seconds");
    _wifiConnectTimer.restart();
  }

}
