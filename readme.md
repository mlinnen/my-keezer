# Overview
This is my controller for my keezer.  


## Goals 
I have a number of goals I want to achieve for this custom controller.  At this point I have no idea if I complete all the goals with a single ESP8266 so I might
break the controller into multiple ESP8266's.  This project will evolve over a long period of time and although I listed all these goals that does not mean the 
goals have all been coded and work.

### Current list of goals
* Measure the temperature inside the keezer with a single temperature sensor 
* Control the keezer temperature given a low/high temp to turn off/on the compressor
* Turn on/off a LED when the compressor is on/off
* LCD display the current temperature of the keezer
* Broadcast current temperature readings via MQTT
* Broadcast when the compressor turns on/off via MQTT
* Set the temperature of the keezer via MQTT
* Measure the temperature inside the keezer with two temperature sensors (one at the top and one at the bottom)
* Turn on/off a fan to circulate the air in the keezer
* Set the statistics of the beer in a keg via MQTT (Tap #, Name, Style, ABV, Date kegged)
* Broadcast the statistics of the beer in each keg via MQTT (Tap #, Name, Style, ABV, Date kegged)
* LCD display the statistics of the beer in each keg (Tap #. Name, Style, ABV, Date kegged)
* Measure the beer flow to keep track of how much beer is left in the keg
* Turn on an LED when the beer is close to empty in a keg
* Broadcast the amount of beer in the keg via MQTT
* LCD display the amount of beer left in the keg
* Measure the amount of Co2 in my tank
* Turn on an LED when the Co2 is getting low
* LCD display the amount of Co2 in my tank
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
