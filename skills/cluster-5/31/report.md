#  LIDAR
Author: Hazim Ab Halim

Date: 2021-04-28
-----

## Summary
This skill is to explore the uses of a LIDAR as a way to measure distances. LIDAR uses I2C and is more accurate than the IR and ultrasonic sensor. The LIDAR requires two read and write commands in order to poll and measure the distances correctly. The readings are displayed onto the terminal via the serial read. 

An important note is that the LIDAR has it's own read address and uses 5V power source

I have the V1 LIDAR which means that in order to get an accurate reading, I need to:
    1) Write to address 0x04
    2) Wait 20ms
    3) Read multiple bits from address 0x8F

## Sketches and Photos
https://drive.google.com/file/d/1c8LhspOkecdqSep-lpYm8wSGogZAF4Z2/view?usp=sharing

## Modules, Tools, Source Used Including Attribution


## Supporting Artifacts


-----
