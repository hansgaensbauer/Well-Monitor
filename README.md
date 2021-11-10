# Well-Monitor
Well monitoring equipment for Engineers Without Borders

This hardware is for projects that require more monitoring than is possible with manual measurements taken by community members. The well monitor is easily programmed to automatically log sensor values, and can be deployed into remote locations that are inconvenient to access regularly. 

![Isometric View of V0](https://github.com/hansgaensbauer/Well-Monitor/blob/main/Hardware/Well%20Depth%20with%20Mains%20Power/Pictures/iso_view_v0.png?raw=true)

![Top View of V0](https://github.com/hansgaensbauer/Well-Monitor/blob/main/Hardware/Well%20Depth%20with%20Mains%20Power/Pictures/top_view_v0.png?raw=true)

## Features
* Onboard SD card for data logging over months/years
* Supports 120 and 240V input
* Onboard RTC w/ battery backup for accurate timestamping even in the event of a power outage
* Support for 24V pressure sensors with current output
* Arduino IDE programmable with USB serial
* Expansion terminal blocks for additional sensors
* Simple, robust construction for long lifetime
* Low BOM cost
* Low power modes

## Optional/Desired Features
* Upload data to the internet
* Integrated PV/battery power supply
* Relay for controlling external HV/HC loads
* Temperature sensor frontend
* Fits into COTS enclosure

## Setup

To view/edit the KiCAD schematic, with KiCAD open, go to Preferences -> Configure Paths, and add a new environment variable called "EWB" that points to Well-Monitor\Hardware\lib. 

## To-Do
* Create installation documentation
* Create Arduino-style pinout diagram
* Implement RTC firmware

## Known Issues
### V0
* Reset button needs silkscreen
* DTR pin needs clamping diodes

