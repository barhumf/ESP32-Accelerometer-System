# ESP32-Accelerometer-System
Embedded system using ESP32 and accelerometer with OLED display

## Overview
This project uses an ESP32 microcontroller and an accelerometer sensor to measure motion and display real-time data on an OLED screen.

## Features
- Real-time acceleration data
- OLED display output
- Push button input
- Stable power regulation

## Hardware Used
- ESP32
- Accelerometer sensor
- OLED display
- Voltage regulator
- Push button

## How It Works
The ESP32 reads acceleration data from the sensor, processes it, and displays the results on the OLED screen.

## Block Diagram
<img width="947" height="745" alt="image" src="https://github.com/user-attachments/assets/aae33f6e-6e78-4908-95ed-9c87c3d1a965" />
Shows the system being powered by a battery and outputing results on the display. Also, the push button being an input to the system and the string controlling the system. 

## Top-Level System Architecture
<img width="962" height="447" alt="image" src="https://github.com/user-attachments/assets/d1367eae-62a5-4518-9550-042c206233c6" />
Shows all basic blocks that are within the system such as the battery that powers it, the voltage regulator that regulates to the desired voltage of the ESP32, the push button which is an input to the system, and the display which is the OLED display and displays proccessed data. 

## Future Improvements
- Wireless data transmission (Bluetooth/WiFi)
- Data logging
