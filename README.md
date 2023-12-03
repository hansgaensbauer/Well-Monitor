# Well Monitor
Well monitoring equipment for Engineers Without Borders

This hardware is for projects that require more monitoring than is possible with manual measurements taken by community members. The well monitor works by weighing a pipe or other weight suspended within the well, since the apparent weight of the pipe is directly proportional to the amount of the pipe left above water.

## Installation and Use
The well monitor consists of two parts: a small "probe" that contains the load cell for weighing the pipe, and a separate box with the battery, USB flash drive, and readout electronics that gets mounted nearby. The probe requires a 3/8" hole to be drilled in the well cap over the location where the pipe is to be suspended. Remove one of the acetyl nuts on the bottom of the probe, thread the stainless-steel cable through the hole in the well cap, and reattach the nut on the other side to secure the probe. Mount the electronics box vertically against a wall or to a pole driven into the ground nearby. 

Since each well installation is different, the well monitor needs to be calibrated during installation:
1. Estimate the amount of pipe required to span the maximum expected variation in water level.
2. Calculate the maximum expected weight. This is the weight of the pipe and the cable used to suspend the pipe
3. Calculate the minimum expected weight. Calculate the buoyant force on the pipe with the water level at its highest expected level, and subtract this from the maximum weight.
4. Fill jugs of water to create "dummy" weights equal to the maximum and minimum weights, and one more equal to the difference between the maximum and minimum weights.
5. Disconnect the battery for the well monitor and remove the plastic cover that covers the electronics within the waterproof box.
6. Using clip leads, apply 9V to the indicated positions on the well monitor.
7. Use a multimeter to measure the voltage on the "Vout" testpoint indicated in the diagram.
8. Turn the "gain" potentiometer all the way to the left (counterclockwise).
9. Turn the "bias" potentiometer until the voltage on the multimeter just begins to increase above zero. Try to get the potentiometer to the very edge of the point where the voltage begins to increase.
10. Suspend the "difference" weight dummy from the well monitor. Ajust the "gain" potentiometer until the voltage reads close to 3.3V (it is clamped so that it cannot exceed this voltage).
11. Suspend the "minimum" weight dummy from the well monitor. Repeat step 9 to set the proper "zero weight" point.
12. Suspend the "maximum" weight dummy from the well monitor. The voltage should be near 3.3V. Make small adjustments to the gain and offset so that the voltage is as close as possible to zero when the minimum weight is attached, and as close as possible to 3.3V when the maximum weight is attached.

Once the battery is connected, measurements are saved once every three hours to a USB flash drive. To read the data, remove the flash drive and connect it to a computer. The file 'welldata.txt' will contain one line for each measurement, consisting of a measurement counter (to keep track of when measurements are restarted) followed by the measurement. The measurements are in "ADC Counts": a reading of 0 corresponds to zero volts, or the maximum water level when the well monitor is calibrated. A value of 32768 corresponds to 3.3V, or the minimum water level when the well monitor is calibrated.
