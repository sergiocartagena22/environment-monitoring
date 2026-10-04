# ESP32 Environmental Monitoring Dashboard
An embedded monitoring system built with the Freenove ESP32-WROVER starter kit.

## Current Features

- Temperature monitoring using a DHT11 sensor
- Humidity monitoring using a DHT11 sensor
- Analog light sensing using a photoresistor
- Relative light-level calculation from ESP32 ADC readings
- Real-time sensor output through the Serial Monitor
- 16x2 LCD output using I2C
- Multiple LCD display pages
- Non-blocking task scheduling using `millis()`

## Hardware

- Freenove ESP32-WROVER
- DHT11 temperature and humidity sensor
- Photoresistor
- 16x2 I2C LCD
- Breadboard
- Jumper wires
- Resistor

## Current Status

### Milestone 1 - Temperature and Humidity Monitoring 
Completed:
- Integrated the DHT11 sensor
- Read temperature and humidity
- Displayed readings through the Serial Monitor
- Displayed readings on the 16x2 LCD

### Milestone 2 - Light Sensor Integration
Completed.
- Integrated a photoresistor using an ESP32 analog input
- Read raw ADC light values
- Converted ADC readings to a relative light percentage
- Displayed light data through the Serial Monitor
- Added a second LCD page for light information
- Implemented independent sensor update intervals using `millis()`

## Planned Features
- PIR motion detection
- Ultrasonic distance sensing
- System status and alert logic
- Wi-Fi web dashboard
- Remote monitoring and control
- FreeRTOS task management
- Data logging
- Improved sensor calibration

## Project Structure

```text
esp32-environment-dashboard/
├── README.md
├── firmware/
│   └── environment_monitor/
│       └── environment_monitor.ino
├── docs/
│   └── progress/
└── notes/
    └── milestones.md
