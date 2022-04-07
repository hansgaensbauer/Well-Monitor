# Well-Monitor
Well monitoring equipment for Engineers Without Borders

This hardware is for projects that require more monitoring than is possible with manual measurements taken by community members. The well monitor is easily programmed to automatically log sensor values, and can be deployed into remote locations that are inconvenient to access regularly. 

## Water Level Sensor
Because COTS pressure transducers are very expensive, we needed an alternative sensing technique for depth measurement. With this device, we measure well depth by weighing a length of HDPE pipe that is suspended in the well - as the water level rises, more of the pipe is submerged, and the apparent weight decreases. The typical configuration of the sensor looks like the following:

![A Model of the Depth Sensor](https://github.com/hansgaensbauer/Well-Monitor/blob/main/Hardware/depth-sensor-cad.png?raw=true)

The Depth Sensor is compatible with a wide variety of cheap strain gauges, and is compatible with any microcontroller with an ADC. It consumes zero power when not in use. In an attempt to reduce the cost as much as possible, the instrumentation amplifier for the strain gauge is implemented using two cheap 358 op amps. 

![Depth Sensor PCB Top View](https://github.com/hansgaensbauer/Well-Monitor/blob/main/Pictures/depth-sensor-top.jpg?raw=true)

![Depth Sensor PCB Bottom View](https://github.com/hansgaensbauer/Well-Monitor/blob/main/Pictures/depth-sensor-bottom.jpg?raw=true)

## Well Depth with Battery \[WIP\]
This device is intended for locations where mains power is not available. It uses a single 9V battery which will last for 10 years. It uses the same strain-guage based sensing technique and is the lowest cost solution available. 

Data is saved once every three hours to a USB flash drive, which is removed and replaced whenever a user wants to view the data. 

## Well Depth with Mains Power
This device is intended for installations that have access to 120-220V 50-60Hz power from a grid or solar installation. Well depth measurements are saved to an SD card, and the device is compatible with standard 4-20mA output water pressure transducers. 

![Isometric View of V1](https://github.com/hansgaensbauer/Well-Monitor/blob/main/Pictures/iso_view_v1.png?raw=true)

![Photo of Assembled V0](https://github.com/hansgaensbauer/Well-Monitor/blob/main/Pictures/photo_v0.jpg?raw=true)

The well monitor uses the CY7C65213A USB-Serial converter. Windows users should go to https://www.cypress.com/documentation/software-and-drivers/usb-serial-software-development-kit (registration/login required) to download and install the drivers. 

The device can be programmed through the Arduino IDE. Under Tools -> Board, select Arduino Uno. 

## Simulation
The WaterSys-PV Notebook can simulate a complete photovoltaic pump installation by combining weather data with simulated conversion, pumping, and usage parameters. 

