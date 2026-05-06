# HMS_BME68X_DRIVER 🌡️

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![Version](https://img.shields.io/badge/Version-1.0.1-green.svg)](https://github.com/Hamas888/HMS_BME68X_DRIVER)
[![Platform](https://img.shields.io/badge/Platform-ARDUINO%20|%20ESP32%20|%20STM32%20|%20nRF52%20|%20Zephyr-orange.svg)](https://github.com/Hamas888/HMS_BME68X_DRIVER)
[![Status](https://img.shields.io/badge/Status-Under%20Development-yellow.svg)]()

A **cross-platform BME68X environmental sensor driver library** for embedded systems that provides accurate temperature, humidity, pressure, and gas resistance measurements with integrated air quality scoring. HMS_BME68X seamlessly adapts to different embedded environments including Arduino, ESP-IDF, STM32 HAL, and nRF Connect SDK (Zephyr).

**Built on**: [Bosch Sensortec's official BME68X Sensor API](https://github.com/BoschSensortec/BME68x-Sensor-API)

## 📑 Table of Contents

- [⚠️ Development Status](#️-development-status)
- [✨ Features](#-features)
- [🎯 Supported Platforms](#-supported-platforms)
- [📦 Installation](#-installation)
  - [PlatformIO (Arduino & ESP-IDF)](#platformio-arduino--esp-idf)
  - [ESP-IDF Component](#esp-idf-component)
  - [Arduino IDE Library Manager](#arduino-ide-library-manager)
  - [nRF Connect SDK (Zephyr)](#nrf-connect-sdk-zephyr)
  - [STM32Cube Project](#stm32cube-project)
- [🚀 Quick Start](#-quick-start)
  - [Arduino Example](#arduino-example)
  - [ESP-IDF Example](#esp-idf-example)
  - [STM32 HAL Example](#stm32-hal-example)
- [📋 Reading Sensor Data](#-reading-sensor-data)
- [⚙️ Configuration](#️-configuration)
  - [Debug Logging](#debug-logging)
  - [I2C Address Selection](#i2c-address-selection)
  - [Gas Sensor Calibration](#gas-sensor-calibration)
- [🔧 Troubleshooting](#-troubleshooting)
- [📁 Repository Structure](#-repository-structure)
- [📄 License](#-license)
- [👨‍💻 Author](#-author)

## ⚠️ Development Status

This library is **actively under development**. Current implementation status:

| Component | Status | Notes |
|-----------|--------|-------|
| **ESP-IDF** | 🟢 Working | Core functionality implemented & tested |
| **STM32 HAL** | 🟡 In Progress | Basic structure ready, full integration coming |
| **Arduino** | 🟡 Planned | Framework detection ready, implementation coming |
| **Zephyr/nRF** | 🟡 Planned | Framework detection ready, implementation coming |
| **Examples** | 🟡 Coming | Will be added in v1.1.0 |
| **Unit Tests** | 🔴 Planned | Scheduled for v1.2.0 |

**Recommended for production**: ESP-IDF only at this time
**Evaluation/Testing**: STM32 HAL (core logic available)

## ✨ Features

- **🌡️ Multi-Sensor Support**: Temperature, humidity, pressure, and gas resistance measurements
- **📊 Air Quality Scoring**: Integrated air quality calculation algorithm with 0-500 AQI scale
- **🎯 Automatic Platform Detection**: Detects and adapts to Arduino, ESP-IDF, STM32 HAL, and Zephyr platforms
- **🧵 Thread-Safe Operation**: Safe concurrent sensor access from multiple tasks/threads
- **🔌 Flexible I2C Interface**: Support for multiple I2C buses and configurable addresses (0x76, 0x77)
- **⚙️ Comprehensive Configuration**: Tunable gas sensor parameters, heater temperature, and humidity calibration
- **📈 Precision Algorithms**: Hardware-optimized calculations for gas reference, humidity scoring, and environmental indices
- **🐛 Optional Debug Logging**: ChronoLog integration for detailed sensor diagnostics (optional)
- **💾 Memory Efficient**: Minimal memory footprint suitable for resource-constrained embedded systems
- **🚀 Zero Configuration**: Works out-of-the-box with sensible defaults
- **📦 Official Package Managers**: Available on Arduino IDE Library Manager and PlatformIO

## 🎯 Supported Platforms

| Platform | Framework | I2C Support | RTOS Support | Status | Notes |
|----------|-----------|-------------|------|--------|-------|
| **ESP32** | ESP-IDF | ✅ ESP-IDF Driver | ✅ FreeRTOS | ✅ Tested | Recommended |
| **ESP32** | Arduino | ✅ Built-in | ✅ FreeRTOS | 🟡 In Progress | Structure ready |
| **STM32** | HAL | ✅ HAL I2C | ✅ FreeRTOS/CMSIS-RTOS | 🟡 In Progress | Core logic ready |
| **STM32** | HAL | ✅ HAL I2C | ❌ Bare Metal | 🟡 In Progress | Core logic ready |
| **nRF52** | nRF Connect SDK | ✅ Zephyr I2C | ✅ Zephyr | 🟡 Planned | Coming soon |
| **Arduino** | Arduino | ✅ Wire/TwoWire | ✅ Optional | 🟡 Planned | Coming soon |

## 📦 Installation

HMS_BME68X is available through multiple package managers. Choose your platform:

> 🟢 **Recommended**: ESP-IDF (fully implemented)
> 🟡 **In Progress**: STM32 HAL, Arduino, Zephyr (structure ready, coming in v1.1.0)

### Arduino IDE Library Manager ⭐ (Easiest for Arduino)

1. Open Arduino IDE → **Sketch** → **Include Library** → **Manage Libraries**
2. Search for "HMS_BME68X_DRIVER"
3. Click **Install**

### PlatformIO (Arduino & ESP-IDF)

#### Method 1: Library Manager Search
Search for "HMS_BME68X_DRIVER" in the PlatformIO Library Manager and add it to your project.

#### Method 2: Add to platformio.ini

```ini
[env:your_board]
platform = your_platform
board = your_board
framework = arduino  ; or espidf

lib_deps = 
    HMS_BME68X_DRIVER
```

**Example for ESP32-C6:**
```ini
[env:esp32-c6-devkitm-1]
platform = https://github.com/tasmota/platform-espressif32.git
board = esp32-c6-devkitm-1
framework = arduino

lib_deps = 
    HMS_BME68X_DRIVER
```

### ESP-IDF Component

1. Create a `components` folder in your project root (if it doesn't exist)
2. Clone the repository:
   ```bash
   cd components
   git clone https://github.com/Hamas888/HMS_BME68X_DRIVER.git
   ```
3. Add the requirement to your main `CMakeLists.txt`:
   ```cmake
   idf_component_register(
       SRCS "main.cpp"
       INCLUDE_DIRS "."
       REQUIRES HMS_BME68X_DRIVER
   )
   ```

### nRF Connect SDK (Zephyr)

1. Create a `modules` folder in your project root
2. Clone the repository:
   ```bash
   cd modules
   git clone https://github.com/Hamas888/HMS_BME68X_DRIVER.git
   ```
3. Add to your root `CMakeLists.txt`:
   ```cmake
   add_subdirectory(modules/HMS_BME68X_DRIVER)
   target_link_libraries(app PRIVATE HMS_BME68X_DRIVER)
   ```

### STM32Cube Project

1. Clone the repository in your project root:
   ```bash
   git clone https://github.com/Hamas888/HMS_BME68X_DRIVER.git
   ```
2. Add to your root `CMakeLists.txt` after `add_executable()`:
   ```cmake
   add_executable(${PROJECT_NAME} ${SOURCES} ${LINKER_SCRIPT})
   
   # Add HMS_BME68X_DRIVER
   add_subdirectory(HMS_BME68X_DRIVER)
   target_link_libraries(${PROJECT_NAME} HMS_BME68X_DRIVER)
   ```

## 🚀 Quick Start

> ⚠️ **Note**: Full platform-specific examples will be added in v1.1.0. Currently, the core sensor reading logic works across all platforms. Examples below show the pattern for each framework.

### Arduino Example

```cpp
#include "HMS_BME68X_DRIVER.h"

HMS_BME68X bme68x;
bme68x_data sensorData;

void setup() {
    Serial.begin(115200);
    
    // Initialize BME68X with default I2C (address 0x76)
    if (bme68x.init() != HMS_BME68X_OK) {
        Serial.println("BME68X initialization failed!");
        return;
    }
    
    // Optional: Set sensor configuration
    bme68x.setHeaterProfile(320, 150);  // Temp: 320°C, Duration: 150ms
    
    Serial.println("BME68X initialized successfully");
}

void loop() {
    bme68x_data data;
    
    // Read sensor data
    if (bme68x.read(&data) == HMS_BME68X_OK) {
        Serial.print("Temp: ");
        Serial.print(data.temperature);
        Serial.print("°C, Humidity: ");
        Serial.print(data.humidity);
        Serial.print("%, Pressure: ");
        Serial.print(data.pressure / 100.0);
        Serial.println(" hPa");
        
        // Get calculated air quality score
        float airQuality = bme68x.getAirQuality();
        Serial.print("Air Quality: ");
        Serial.println(airQuality);
    }
    
    delay(1000);
}
```

### ESP-IDF Example

```cpp
#include "HMS_BME68X_DRIVER.h"
#include "driver/i2c.h"

#define I2C_MASTER_SCL_IO 22
#define I2C_MASTER_SDA_IO 21
#define I2C_MASTER_NUM I2C_NUM_0
#define I2C_MASTER_FREQ_HZ 100000

HMS_BME68X bme68x;

void i2c_init() {
    i2c_config_t conf = {};
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    
    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
}

void app_main() {
    i2c_init();
    
    // Set I2C port for BME68X
    bme68x.setI2cPort(I2C_MASTER_NUM);
    
    // Initialize sensor
    if (bme68x.init() != HMS_BME68X_OK) {
        printf("BME68X initialization failed!\n");
        return;
    }
    
    bme68x_data data;
    while (1) {
        if (bme68x.read(&data) == HMS_BME68X_OK) {
            printf("Temp: %.2f°C, Humidity: %.2f%%, Pressure: %.2f hPa\n",
                   data.temperature, data.humidity, data.pressure / 100.0);
            
            float airQuality = bme68x.getAirQuality();
            printf("Air Quality Score: %.2f\n", airQuality);
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```

### STM32 HAL Example

> ✅ **Status**: Core functionality available, full integration in progress (v1.1.0)

```cpp
#include "HMS_BME68X_DRIVER.h"

extern I2C_HandleTypeDef hi2c1;  // From CubeMX

HMS_BME68X bme68x;
bme68x_data sensorData;

void init_bme68x() {
    // Set I2C handle
    bme68x.setI2cHandle(&hi2c1);
    
    // Initialize sensor
    if (bme68x.init() != HMS_BME68X_OK) {
        Error_Handler();
    }
}

void read_sensor_task() {
    while (1) {
        if (bme68x.read(&sensorData) == HMS_BME68X_OK) {
            printf("Temp: %.2f°C\n", sensorData.temperature);
            printf("Humidity: %.2f%%\n", sensorData.humidity);
            printf("Pressure: %.2f hPa\n", sensorData.pressure / 100.0);
            printf("Gas: %.2f Ohms\n", sensorData.gas_resistance);
            
            float aq = bme68x.getAirQuality();
            printf("Air Quality: %.2f\n", aq);
        }
        osDelay(1000);
    }
}
```

## 📋 Reading Sensor Data

The `read()` function returns sensor measurements in the `bme68x_data` structure:

```cpp
struct bme68x_data {
    float temperature;      // °C
    float humidity;         // % RH
    float pressure;         // Pa
    float gas_resistance;   // Ohms
    uint8_t gas_range;
    uint8_t gasm_range;
    uint8_t status;
    uint8_t meas_index;
};
```

### Air Quality Scoring Functions

- `getAirQuality()` - Returns combined air quality score (0-500)
- `getGasScore()` - Gas resistance score (0-75%)
- `getHumidityScore()` - Humidity compensation score (0-25%)
- `getGasReference()` - Recalibrate gas reference value

## ⚙️ Configuration

All configuration options are in [HMS_BME68X_Config.h](include/HMS_BME68X_Config.h):

### Debug Logging

Enable optional debug output via ChronoLog:

```cpp
// In HMS_BME68X_Config.h
#define HMS_BME68X_DEBUG_ENABLED 1  // 0 = disabled, 1 = enabled
```

**Requires**: [ChronoLog library](https://github.com/Hamas888/ChronoLog)

### I2C Address Selection

```cpp
#define HMS_BME68X_DEVICE_ADDR1 0x76  // Primary address
#define HMS_BME68X_DEVICE_ADDR2 0x77  // Alternative address
```

### Gas Sensor Calibration

```cpp
#define HMS_BME68X_GAS_REFERENCE        250000.0f   // Reference gas resistance (Ω)
#define HMS_BME68X_GAS_LOWER_LIMIT      5000.0f     // Poor air quality threshold
#define HMS_BME68X_GAS_UPPER_LIMIT      50000.0f    // Good air quality threshold
#define HMS_BME68X_HUMIDITY_REFERENCE   40.0f       // Optimal humidity (%)
```

### Heater Profile Configuration

```cpp
#define HMS_BME68X_HEATER_TEMP_DEFAULT  320   // °C
#define HMS_BME68X_HEATER_DUR_DEFAULT   150   // milliseconds
#define HMS_BME68X_AMBIENT_TEMP_DEFAULT 30    // °C
```

## 🔧 Troubleshooting

### Sensor Not Detected

- Verify I2C address (0x76 or 0x77) matches your hardware
- Check I2C bus voltage and pullup resistors (typically 4.7kΩ)
- Confirm SCL/SDA pins are correctly connected
- Try alternative address: `bme68x.setAddress(0x77)`

### Unrealistic Readings

- Allow 5-10 minutes warmup time for gas sensor stabilization
- Calibrate gas reference: `bme68x.getGasReference()` during initialization
- Verify sensor is not in direct sunlight or near heat sources
- Check I2C clock frequency (100kHz typical for BME68X)

### I2C Communication Errors

- Add logic analyzer to verify I2C signals
- Check for I2C conflicts with other devices on bus
- Reduce I2C speed if using long wires: set to 100kHz
- For ESP-IDF: Verify GPIO matrix configuration

### ChronoLog Not Found

ChronoLog is optional. If debug logging is not needed:
```cpp
#define HMS_BME68X_DEBUG_ENABLED 0  // Disable in HMS_BME68X_Config.h
```

To enable debug logging, install ChronoLog first:
- [ChronoLog on GitHub](https://github.com/Hamas888/ChronoLog)
- [ChronoLog on PlatformIO](https://platformio.org/lib/show/17689/ChronoLog)

## 📁 Repository Structure

```
HMS_BME68X_DRIVER/
├── CMakeLists.txt              # CMake build configuration
├── library.json                # PlatformIO manifest
├── library.properties          # Arduino IDE manifest
├── idf_component.yml           # ESP-IDF component descriptor
├── README.md                   # This file
├── LICENSE                     # MIT License
├── include/
│   ├── HMS_BME68X_DRIVER.h     # Main driver header
│   └── HMS_BME68X_Config.h     # Configuration options
├── src/
│   └── HMS_BME68X_DRIVER.cpp   # Driver implementation
└── BME68x_SensorAPI/           # Bosch Sensortec official BME68X API
    ├── bme68x.h
    ├── bme68x.c
    ├── bme68x_defs.h
    ├── LICENSE                 # Bosch license (BSD 3-Clause)
    └── README.md
```

**Note**: `examples/` folder will be added in v1.1.0 with platform-specific code samples.

## 📄 License

This project is licensed under the **MIT License** - see [LICENSE](LICENSE) file for details.

### Third-Party Licenses

**BME68x_SensorAPI** (in `BME68x_SensorAPI/` folder):
- License: **BSD 3-Clause License**
- Source: [Bosch Sensortec BME68x-Sensor-API](https://github.com/BoschSensortec/BME68x-Sensor-API)
- Copyright: © Bosch Sensortec GmbH
- See `BME68x_SensorAPI/LICENSE` for full terms

### Summary

- **HMS_BME68X_DRIVER wrapper**: MIT License (you can use, modify, distribute freely)
- **Bosch BME68X Sensor API**: BSD 3-Clause (credit required, commercial use allowed)

When using this library, please acknowledge:
1. HMS_BME68X_DRIVER by Hamas Saeed
2. Bosch Sensortec for the official BME68X Sensor API

```
MIT License

Copyright (c) 2025 Hamas Saeed

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, subject to the following conditions...
```

## 👨‍💻 Author

**Hamas Saeed** - Embedded Systems Engineer

- 📧 Email: [hamasaeed@gmail.com](mailto:hamasaeed@gmail.com)
- 🐙 GitHub: [Hamas888](https://github.com/Hamas888)

---

<div align="center">

**If HMS_BME68X_DRIVER helped your project, consider giving it a ⭐**

[Report Issue](https://github.com/Hamas888/HMS_BME68X_DRIVER/issues) • [Request Feature](https://github.com/Hamas888/HMS_BME68X_DRIVER/issues) • [Sponsor](https://github.com/sponsors/Hamas888)

</div>