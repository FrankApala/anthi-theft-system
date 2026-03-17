# Accelerometer, RTC and DAC Project - STM32F407

## General Context
This repository contains an embedded lab project built around the STM32F407 board.
The goal is to validate a complete microcontroller processing chain:
- sensor data acquisition
- application-level processing
- local output (display/serial)
- analog output generation
- time tagging with RTC

It is meant as a practical integration exercise where multiple peripherals work together in one simple real-time application.

## Learning Goals
The main objective is to practice:
- CubeMX/CubeIDE project configuration
- HAL-based peripheral usage across multiple buses (I2C, SPI, USART)
- sensor reading and data usage
- RTC-based time synchronization
- DAC-based analog output generation

## Functional Scope
The application is centered around three main functional blocks:
- Accelerometer: motion/orientation acquisition
- RTC: date/time management
- DAC: analog signal generation tied to application logic

Around these blocks, communication interfaces (serial, sensor buses, display) are used for monitoring and debugging.

## High-Level Structure
The repository follows the standard STM32CubeIDE layout:
- application code in Core
- vendor layers in Drivers
- hardware configuration in the .ioc file

## Quick Start
1. Open the project in STM32CubeIDE.
2. Select and verify the Debug configuration.
3. Build and flash the STM32F407 target board.
4. Observe runtime behavior using serial output and/or the display.

## Maintenance Notes
- Keep custom logic inside USER CODE sections.
- Keep the .ioc configuration consistent with generated code.
- Keep this README focused on context, goals, and global behavior.
