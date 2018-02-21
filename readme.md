# Overview
This is my controller for my keezer.  


## Goals 
I have a number of goals I want to achieve for this custom controller.  At this point I have no idea if I complete all the goals with a single ESP8266 so I might
break the controller into multiple ESP8266's.  This project will evolve over a long period of time and although I listed all these goals that does not mean the 
goals have all been coded and work.

### Current list of goals
* Control the keezer temperature given a temperature set point and the temperature sensor reading
* Turn on/off a LED when the compressor is on/off
* Display the current temperature of the keezer on an LCD
* Broadcast current temperature readings via MQTT
* Broadcast when the compressor turns on/off via MQTT
* Set the temperature of the keezer via MQTT
* Measure the temperature inside the keezer with two temperature sensors (one at the top and one at the bottom)
* Turn on/off a fan to circulate the air in the keezer
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
* Detect motion when someone approaches the keezer
* Turn on LED lighting when someone approaches and turn off the lighting after a delay
* Alexa tell you about the beers on tap

## Features
This is a list of actual features the current code base supports.  Eventually the goals will turn into features.

### Current list of features
* NONE - I am just getting started

## Hardware
