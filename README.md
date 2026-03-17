# Anti-Theft System with Motion Detection - STM32F407

## General Context
This repository contains an embedded anti-theft system built around the STM32F407 board.
The system focuses on low-power monitoring and alerting, with three core capabilities:
- accelerometer-based motion detection
- PWM buzzer audio alerts when suspicious movement is detected
- optimized UART usage to reduce power consumption during communication/debug phases

The project is designed as a practical integration exercise that combines sensing, decision logic, and power-aware signaling in one application.

## Learning Goals
The main objective is to practice:
- CubeMX/CubeIDE configuration for an embedded security use case
- reliable motion detection from accelerometer data
- PWM generation for buzzer-based alert tones
- UART optimization strategies for lower energy use
- robust system behavior under real-time constraints

## Functional Scope
The application is centered around three functional blocks:
- Motion sensing: detect movement patterns using the accelerometer
- Alerting: trigger audible buzzer alarms through PWM control
- Low-power communication: keep UART available while minimizing energy overhead

Supporting interfaces are used for configuration, monitoring, and debugging.

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
