# Microcontroller-Based Heart Rate Monitor

## Overview
This repository contains the bare-metal C programming codebase for a heart rate monitoring system, developed as the term project for the EE308 Microcomputer Based System Design course.

## Hardware & Peripherals
* **Sensor:** MAX30102 Pulse Oximeter and Heart-Rate Sensor
* **Input/Output:** Keypad and Display integration
* **Communication Protocols:** I2C (for sensor communication) and USART (for serial data transmission)

## Project Structure
The project is modularized into several libraries:
* `max30102.c` / `.h`: Sensor driver implementation
* `i2c.c` / `.h`: I2C protocol management
* `usart0.c` / `.h`: Serial communication handling
* `keypad.c` / `.h` & `display.c` / `.h`: User interface logic
