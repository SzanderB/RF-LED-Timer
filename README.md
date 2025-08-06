# Personal Project 1: LED Timer Bar

Video Link: https://youtube.com/shorts/uT_QXEIdz4I

## Description
Using an ardiuno uno, a buzzer, IR reciever, and neopixel LEDs, I was able to make an accurate and responsive timer bar. Using a random TV remote, you can program the commands for different countdown times and have the arduino count down for you.
It uses the FastLED library to control the LEDs and the IRRemote library to receive IR commands. 

The whole thing is powered with an external 5V wall plug. The reason this isnt battery powered is that I am using 60 LEDs which can each draw ~60mA at full bright white. While I won't be doing full bright white, batteries won't last very long even running at only 0.5 A and I don't want to consistently have to spend money on it.

## Purpose
When stretching, I often need a consistent timer (I apparently suck at counting in my head). Therefore, I wanted something adjustable and easy to use to count time for me.

## Methods
I believed that using a large LED strip would make this timer very visible, no matter where its placed. It can be colored to imply time left, as well as have an hourglass effect of time running out.
For control, I originally opted for manual setting. To set the time, scroll through a potentiometer, and click a button to start/stop the timer.

After creating the first rendition and trying it out, i realized how annoying a physical control was during stretching. Each time I wanted the timer, I'd have to get up and change it/click the button. However, the LED Panel was bright and was a good indicator of the time.

However, the LED bar wasn't helpful if my back is to it. So, in the next rendition, I journeyed to solve those issues. Instead of a physical button/potentiometer, I used a RF receiver and a remote to wirelessly transmit commands. Furthermore, I added a buzzer that would sound each time the clock ran out.

## Issues
Arduinos are great because there is a lot of prewritten code for them (FastLED and IRRemote). However, they are only 8-bit uCs and only have 1 core. The FastLED command .show(), which displays the LEDs, turns off interrupts, which IRRemote relies on to get data. So, if I wanted to refresh my LEDs at 10Hz, then my IRRemote is very unlikely to receive a whole transmission without getting interrupted. Therefore, if I had 2 cores, like in a raspberry pi pico, I could do both without having to worry about one interrupting the other. My workaround for this is refresh LEDs at 4Hz. Its noticable, but doesn't impact functionality.

## How to use
I've attatched the schematics as well as a BOM to follow along. Create the schematic and make sure to check connections along the way (saves time debugging later). Download the code and try it out!
