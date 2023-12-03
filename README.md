# Well-Monitor
Well monitoring equipment for Engineers Without Borders

This hardware is for projects that require more monitoring than is possible with manual measurements taken by community members. The well monitor can be deployed into remote locations that are inconvenient to access regularly. 

## Well Depth with Battery \[WIP\]
This device is intended for locations where mains power is not available. It uses a single 9V battery which will last for 10 years. It uses the same strain-guage based sensing technique and is the lowest cost solution available. 

Data is saved once every three hours to a USB flash drive, which is removed and replaced whenever a user wants to view the data. 

The device seals inside a standard 2" PVC pipe. The center photo shows the inner assembly, with the PCB on the right.
<p float="left">
  <img src="https://github.com/hansgaensbauer/Well-Monitor/blob/main/Pictures/Outside.png?raw=true" height="400">

  <img src="https://github.com/hansgaensbauer/Well-Monitor/blob/main/Pictures/Assembly.png?raw=true" height="400">
  
  <img src="https://github.com/hansgaensbauer/Well-Monitor/blob/main/Pictures/wdb-pcb.png?raw=true" height="400">
 </p>

## Water Level Sensor
Because COTS pressure transducers are very expensive, we needed an alternative sensing technique for depth measurement. With this device, we measure well depth by weighing a length of HDPE pipe that is suspended in the well - as the water level rises, more of the pipe is submerged, and the apparent weight decreases. The typical configuration of the sensor looks like the following:

![A Model of the Depth Sensor](https://github.com/hansgaensbauer/Well-Monitor/blob/main/Pictures/depth-sensor-cad.png?raw=true)

The Depth Sensor is compatible with a wide variety of cheap strain gauges, and is compatible with any microcontroller with an ADC. It consumes zero power when not in use. In an attempt to reduce the cost as much as possible, the instrumentation amplifier for the strain gauge is implemented using two cheap 358 op amps. 
<p float="left">
  <img src="https://github.com/hansgaensbauer/Well-Monitor/blob/main/Pictures/depth-sensor-top.jpg?raw=true" height="300">

  <img src="https://github.com/hansgaensbauer/Well-Monitor/blob/main/Pictures/depth-sensor-bottom.jpg?raw=true" height="300">
 </p>
