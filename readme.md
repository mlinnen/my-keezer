# Overview
This is my custom controller for my keezer.  I am building this from the ground up because I have some very specific ideas that I want to include and I have found no other solution (custom or off the self) that meets my needs.

## Build status
Master Branch [![Build Status](https://travis-ci.org/mlinnen/my-keezer.svg?branch=master)](https://travis-ci.org/mlinnen/my-keezer) 
Dev Branch [![Build Status](https://travis-ci.org/mlinnen/my-keezer.svg?branch=dev)](https://travis-ci.org/mlinnen/my-keezer)

## Goals 
I have a number of goals I want to achieve for this custom controller.  At this point I doubt all of this could or should be done on a single ESP8266 micro.  It is likely I will have a number of ESP8266's that work together over MQTT to achieve all of the functionality I want.  This project will evolve over a long period of time.  The following is a list of goals which I hope to eventually turn into actual features.

### Future list of features
Check out the [Github Issues (label enhancements)](https://github.com/mlinnen/my-keezer/labels/enhancement) for a list of features that are planned for a later date.

## Features
This is a list of actual features the current code base supports.

### Current list of features
* Measure the temperature inside the keezer with two temperature sensors (one at the top and one at the bottom)
* Control the keezer temperature given a temperature set point and the temperature sensor reading ()
* Display the current temperature of the keezer on an LCD
* Turn on/off a fan(s) to circulate the air in the keezer when the difference between the top and bottom sensors is greater than a given threshold.  
* Detect motion when someone approaches the keezer
* Turn on LED lighting when someone approaches and turn off the lighting after a delay
* Connects to MQTT broker using a user name and password
* Broadcast over MQTT sensor readings such as: temperature and motion
* Broadcast over MQTT event changes such as: compressor, fan and lights turning on/off
* Respond to an MQTT ping request so that an external application can determine what keezers are online
* Allow the keezer to join a wifi network without changing the code

## Hardware
* 2 DS18B20 Temperature Sensor
* 1 4.7K Resistor
* 1 [HUZZAH ESP8266](https://www.adafruit.com/product/2471)
* 1 [4 Channel Relay Board](https://www.sunfounder.com/4-channel-5v-relay-shield-module.html)
* 1 [4 x 20 LCD](https://www.adafruit.com/product/498)
* 1 [I2C / SPO character LCD backpack](https://www.adafruit.com/product/292)
* 1 [12v 5A switching power supply](https://www.adafruit.com/product/352)
* 1 [Mini DC/DC Converter 5v @ 1A](https://www.adafruit.com/product/1065)
* 1 [LED Strip Light Triangle Bulbs](https://www.amazon.com/Triangle-Bulbs-T93003-Waterproof-Flexible/dp/B00IZA2URS)

## Wiring
I used Fritzing to document the wiring of all components.  After installing Fritzing just open the doc\keezer.fzz file from Fritzing.  Or you can look at the doc\Wiring.png file.

## I/O
* 1 gpio for 1 wire for temperature sensors
* 1 gpio for relay dc fan
* 1 gpio for relay LED light strip
* 1 gpio for relay compressor
* 1 gpio for motion sensor
* 2 gpio for LCD
* 1 gpio for push button LCD mode

### LCD Display layout
A push button allows you to change the mode of the LCD.  Mode 1 is the default mode and after a certain amount of time the LCD will fall back into this mode.

Example Mode 1 Top, Average & Bottom Compressor on
```
Current Temperature
Set  43.0 Top 44.5
          Avg 43.5
Comp On   Bot 42.5
```  
Example Mode 1 Top, Average & Bottom Compressor off
```
Current Temperature
Set  43.0 Top 44.5
          Avg 43.5
Comp Off  Bot 42.5
```  

Example Mode 2 Set Temperature
```
Set Temperature
Top 44.0
Avg 43.0
Bot 42.0
```

## Dependencies
This application depends on the following libraries:
* OneWire
* DallasTemperature
* RBD_Timer
* RBD_Button
* Adafruit LiquidCrystal
* PubSubClient
* WifiManager
* ArduinoJson

## Build
There are a couple things you need to do so that you can compile the project. I converted this project to use [Platform IO](https://platformio.org/) so you will need to install it as well as an editor. I personally think that the Visual Studio code editor works very well with Platform IO.

1. Make a copy of the myconfig.sample.h and rename it myconfig.h
2. Edit the myconfig.h and set the specific topics that you want the keezer to use (or leave them as is).  If you have more than 1 keezer make sure you changed the integer in all the topics so that one keezer can be addressed differently than the other keezer
3. You can also make changes to the default settings in myconfig.h
4. Place the ESP8266 into flash mode
5. Run the following command 
        pio run --target upload
6. The first time that the application is downloaded to the target platform it will put the device in an Access Point mode which will allow you to configure it for connection to your Wifi
7. Open your Wifi and connect to "keezer-ap". Note you have 120 seconds after power on to complete this task before the keezer falls out of AP mode and starts controlling the temperature of the beer.  If you dont complete this process before the 120 second timeout simply cycle the power on the keezer and try again
8. Click on the Wifi setup button and fill in the text fields.  Once the information is submitted the device will connect to Wifi and the MQTT broker

## MQTT
The only way to communicate with the keezer is to use MQTT messages.  Currently the integration is mostly one way (keezer publishing messages to MQTT), but at some point you will be able to control aspects of the keezer by sending messages to it.  If you have never used MQTT before then I would advise you to head on over to [MQTT.org](http://mqtt.org/) to learn more about it.

**Note:** This is a work in progress and I am likely going to re-factor these MQTT topics and message formats as I evolve the application over time.

## Topics and Messages
The Topics and Messages that the keezer uses will be defined here so that you have one place for determining how to interact with it.  You can also edit the mymqttbroker.h file to customize how you want your topics to be defined on the MQTT bus.  For instance not everyone will want their MQTT topics to start with /home/keezer like I have put together.  Hopefully this meets everyones needs on allowing the keezer to exist along with any other MQTT topics you have on your network.

### MQTT Subscriptions
The following subscriptions are supported by the keezer.

#### Keezer ping
This allows some external application to request what keezers are on the bus.  Each keezer (if you have multiple) will respond to this ping request.
``` 
/home/keezer/ping 
```

### MQTT Publications

#### Keezer ping response
In response to the ping request each keezer will publish the following.  Where {id} is the unique id (integer) of the keezer.  This allows for more than one keezer to exist on the bus.
``` 
/home/keezer/pingr/{id} 
```

#### Keezer temperatures
The keezer will broadcast the current temperature readings.  Where {id} is the unique id (integer) of the keezer.  The body of the message contains a floating point number that represents Fahrenheit.  There are multiple temperature readings that the keezer tracks: Average, Top and Bottom. 
``` 
/home/keezer/{id}/temp/avg {float}
/home/keezer/{id}/temp/top {float}
/home/keezer/{id}/temp/bottom {float}
```

#### Keezer motion detection
The keezer will broadcast the status of the motion detector when it changes.  Where {id} is the unique id (integer) of the keezer.  The body of the message contains a 1 or 0 indicating if there was motion detected (1) or motion has stopped being detected (0). 
``` 
/home/keezer/{id}/motion/value 1
/home/keezer/{id}/motion/value 0
```

#### Keezer compressor status
The keezer will broadcast when the compressor is turned on/off.  Where {id} is the unique id (integer) of the keezer.  The body of the message contains a 1 or 0 indicating if the compressor was turned on (1) or off (0).
``` 
/home/keezer/{id}/compressor/value 1
/home/keezer/{id}/compressor/value 0
```

#### Keezer fan status
The keezer will broadcast when the fan(s) is turned on/off.  Where {id} is the unique id (integer) of the keezer.  The body of the message contains a 1 or 0 indicating if the fan was turned on (1) or off (0).
``` 
/home/keezer/{id}/fan/value 1
/home/keezer/{id}/fan/value 0
```

#### Keezer light status
The keezer will broadcast when the light is turned on/off.  Where {id} is the unique id (integer) of the keezer.  The body of the message contains a 1 or 0 indicating if the light was turned on (1) or off (0).
``` 
/home/keezer/{id}/light/value 1
/home/keezer/{id}/light/value 0
```
