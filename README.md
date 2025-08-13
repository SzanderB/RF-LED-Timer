# LED Timer Bar

[🎥 Video Demo](https://youtube.com/shorts/uT_QXEIdz4I)

## Overview
A wireless, Arduino-based countdown timer using a Neopixel LED strip and buzzer.  
Designed to provide clear, visible, and audible timing cues for stretching or other activities without needing physical interaction.

## Features
- Large 60-LED Neopixel strip with color-coded countdown and hourglass effect
- Wireless control via IR remote (program any commands for different countdown times)
- Audible buzzer alert at timer completion
- Powered by 5V wall adapter for high-current LED operation

## Tech Stack / Components
- **Microcontroller:** Arduino Uno  
- **LEDs:** 60 Neopixels controlled with FastLED library  
- **Control:** IR remote with IRRemote library  
- **Audio:** Buzzer  
- **Power:** External 5V wall adapter

## Development Notes
- Iterated from **manual control** (potentiometer + button) to **wireless IR control** with buzzer.  
- Learned to balance LED refresh rate (4 Hz) with IR input timing to avoid missed commands due to Arduino’s single-core limitations.  
- Optimized LED effects for visibility and intuitive time display.

## Key Learnings
- Hands-on iteration improves usability as much as code functionality.  
- Single-core microcontrollers require careful timing management when combining high-frequency I/O tasks.  
- Designing for real-world use cases (visibility, hands-free operation) informs hardware and software choices.
