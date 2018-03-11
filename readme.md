# Overview
This is my custom controller for my keezer.  I am building this from the ground up because I have some very specific ideas that I want to include and I have found no other solution (custom or off the self) that meets my needs.

## Build status
Master Branch [![Build Status](https://travis-ci.org/mlinnen/my-keezer.svg?branch=master)](https://travis-ci.org/mlinnen/my-keezer) 
Dev Branch [![Build Status](https://travis-ci.org/mlinnen/my-keezer.svg?branch=dev)](https://travis-ci.org/mlinnen/my-keezer)

## Goals 
I have a number of goals I want to achieve for this custom controller.  At this point I doubt all of this could or should be done on a single ESP8266 micro.  It is likely I will have a number of ESP8266's that work together over MQTT to achieve all of the functionality I want.  This project will evolve over a long period of time.  The following is a list of goals which I hope to eventually turn into actual features.

### Current list of goals
* Turn on/off an LED when the compressor is on/off
* Broadcast current temperature readings via MQTT
* Broadcast when the compressor turns on/off via MQTT
* Set the temperature of the keezer via MQTT
* Set the statistics of the beer in a keg via MQTT (Tap #, Name, Style, ABV, Date kegged)
* Broadcast the statistics of the beer in each keg via MQTT (Tap #, Name, Style, ABV, Date kegged)
* Display the statistics of the beer in each keg (Tap #. Name, Style, ABV, Date kegged) on an LCD
* Measure the beer flow to keep track of how much beer is left in the keg
* Turn on an LED when the beer is close to empty in a keg
* Broadcast the amount of beer in the keg via MQTT
* Display the amount of beer left in the keg on an LCD
* Measure the amount of Co2 in my tank
* Turn on an LED when the Co2 is getting low
* Display the amount of Co2 in my tank on an LCD
* Broadcast the amount of Co2 in my tank over MQTT
* Display some of the keezer statistics on my Home Automation system.
* Alexa tell you about the beers on tap

## Features
This is a list of actual features the current code base supports.  Eventually the goals listed above will turn into features here.

### Current list of features
* Measure the temperature inside the keezer with two temperature sensors (one at the top and one at the bottom)
* Control the keezer temperature given a temperature set point and the temperature sensor reading ()
* Display the current temperature of the keezer on an LCD
* Turn on/off a fan to circulate the air in the keezer
* Detect motion when someone approaches the keezer
* Turn on LED lighting when someone approaches and turn off the lighting after a delay

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

## I/O
* 1 gpio for 1 wire for temperature sensors
* 1 gpio for relay dc fan
* 1 gpio for relay LED light strip
* 1 gpio for SSR compressor
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
* Adafruit_LiquidCrystal

## Build
There are a couple things you need to do before compiling the keezer.ino file.

1. Make a copy of the config.sample.h and rename it config.h
2. Edit the config.h and set the variables you want to change in this file.
3. Compile and upload the sketch to your ESP8266 