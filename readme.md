# Overview
This is my custom controller for my keezer.  I am building this from the ground up because I have some very specific ideas that I want to include and I have found no other solution (custom or off the self) that meets my needs.

## Build status
Master Branch [![Build Status](https://travis-ci.org/mlinnen/my-keezer.svg?branch=master)](https://travis-ci.org/mlinnen/my-keezer) 
Dev Branch [![Build Status](https://travis-ci.org/mlinnen/my-keezer.svg?branch=dev)](https://travis-ci.org/mlinnen/my-keezer)

## Goals 
I have a number of goals I want to achieve for this custom controller.  At this point I doubt all of this could or should be done on a single ESP8266 micro.  It is likely I will have a number of ESP8266's that work together over MQTT to achieve all of the functionality I want.  This project will evolve over a long period of time.  The following is a list of goals which I hope to eventually turn into actual features.

### Current list of goals
* Turn on/off an LED when the compressor is on/off
* Display the current temperature of the keezer on an LCD
* Broadcast current temperature readings via MQTT
* Broadcast when the compressor turns on/off via MQTT
* Set the temperature of the keezer via MQTT
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
This is a list of actual features the current code base supports.  Eventually the goals listed above will turn into features here.

### Current list of features
* Measure the temperature inside the keezer with two temperature sensors (one at the top and one at the bottom)
* Control the keezer temperature given a temperature set point and the temperature sensor reading ()

## Hardware

## Dependencies
This application depends on the following libraries:
* OneWire
* DallasTemperature
* RBD_Timer

