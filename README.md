# STM32 Environment Logger

An STM32-based environment logger that reads temperature and pressure from a BMP280 sensor every second, stores the data in an external EEPROM, and displays live readings on an SSD1306 OLED display.

## Hardware Used

- STM32 Nucleo-F446RE
- BMP280 Temperature/Pressure Sensor
- SSD1306 OLED Display (128x64)
- AT24C256 EEPROM

## Features

- 1-second timer interrupt
- BMP280 temperature measurement
- BMP280 pressure measurement
- EEPROM data logging
- SSD1306 OLED display
- Custom I2C driver
- Custom timer driver
- Custom EEPROM driver
- Custom BMP280 sensor interface

## Build Environment

- STM32CubeIDE
- STM32F446RE
- Bare-metal drivers (No HAL)

## Software Architecture

### Drivers

- CMSIS
- GPIO
- RCC
- SYSTICK
- DELAY
- I2C
- TIM
- NVIC
- UART

### BSP

- BMP280
- EEPROM
- FONTS
- LOGGER
- SSD1306

### Application

- Environment Logger

## Current Operation

1. Timer generates an interrupt every second.
2. BMP280 temperature and pressure are read.
3. Measurements are stored in EEPROM.
4. Latest values are displayed on the OLED.

## Future Improvements

- UART command parser
- EEPROM log retrieval over UART
- EEPROM erase command
- Circular buffer logging

## Demo

### Hardware Setup

![Hardware Setup](images/hardware_setup.jpeg)

### OLED Output

![OLED Display](images/oled_display.jpeg)

## Author

Leo Raju