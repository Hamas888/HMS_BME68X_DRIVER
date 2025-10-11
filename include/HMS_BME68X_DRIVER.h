   /*
 ====================================================================================================
 * File:        HMS_BME68X_DRIVER.h
 * Author:      Hamas Saeed
 * Version:     Rev_1.0.0
 * Date:        Oct 9 2025
 * Brief:       This Package Provide BME68X Sensor Driver Library for Cross Platform (STM/ESP/nRF)
 * 
 ====================================================================================================
 * License: 
 * MIT License
 * 
 * Copyright (c) 2025 Hamas Saeed
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * For any inquiries, contact Hamas Saeed at hamasaeed@gmail.com
 *
 ====================================================================================================
 */

#ifndef HMS_BME68X_DRIVER_H
#define HMS_BME68X_DRIVER_H

#if defined(ARDUINO)                                                                                       // Platform detection
    #define HMS_BME68X_PLATFORM_ARDUINO
#elif defined(ESP_PLATFORM)
    #define HMS_BME68X_PLATFORM_ESP_IDF
#elif defined(__ZEPHYR__)
    #define HMS_BME68X_PLATFORM_ZEPHYR
#elif defined(__STM32__)
    #define HMS_BME68X_PLATFORM_STM32_HAL
#endif

#if defined(HMS_BME68X_PLATFORM_ARDUINO)
    #include <Arduino.h>
#elif defined(HMS_BME68X_PLATFORM_ESP_IDF)
    #include <stdio.h>
    #include <stdint.h>
#elif defined(HMS_BME68X_PLATFORM_ZEPHYR)
    #include <stdio.h>
    #include <stdint.h>
    #include "bme68x.h"
    #include "bme68x_defs.h"
    #include <zephyr/device.h>
    #include <zephyr/drivers/i2c.h>
#elif defined(HMS_BME68X_PLATFORM_STM32_HAL)
    #include "main.h"
    #include <stdio.h>
    #include <stdint.h>
    #include "bme68x.h"
    #include "bme68x_defs.h"
    #if defined(osCMSIS) || defined(FREERTOS)
        #define HMS_BME68X_STM32_FREERTOS
    #endif
#endif

#include "HMS_BME68X_Config.h"

#if defined(HMS_BME68X_DEBUG_ENABLED) && (HMS_BME68X_DEBUG_ENABLED == 1)
    #define HMS_BME68X_LOGGER_ENABLED
#endif


typedef enum {
    HMS_BME68X_OK           = 0x00,
    HMS_BME68X_ERROR        = 0x01,
    HMS_BME68X_BUSY         = 0x02,
    HMS_BME68X_TIMEOUT      = 0x03,
    HMS_BME68X_NOT_FOUND    = 0x04
} HMS_BME68X_StatusTypeDef;

class HMS_BME68X {
    public:
        HMS_BME68X();
        ~HMS_BME68X();


        float getGasScore();
        float getAirQuality();
        float getGasReference();
        float getHumidityScore();
        void read(struct bme68x_data *data);

        struct bme68x_data *getSensorData() { return &sensorData; }
        
    #if defined(HMS_BME68X_PLATFORM_ARDUINO)
        HMS_BME68X_StatusTypeDef begin(
            TwoWire *theWire = &Wire, uint8_t addr = HMS_BME68X_DEVICE_ADDR
        );
    #elif defined(HMS_BME68X_PLATFORM_ESP_IDF)
        HMS_BME68X_StatusTypeDef begin(
            i2c_port_t i2c_port = I2C_NUM_0, uint8_t addr = HMS_BME68X_DEVICE_ADDR
        );
    #elif defined(HMS_BME68X_PLATFORM_ZEPHYR)
        HMS_BME68X_StatusTypeDef begin(
            const struct device *i2c_dev = NULL, uint8_t addr = HMS_BME68X_DEVICE_ADDR
        );
    #elif defined(HMS_BME68X_PLATFORM_STM32_HAL)
        HMS_BME68X_StatusTypeDef begin(
            I2C_HandleTypeDef *hi2c = NULL, uint8_t addr = HMS_BME68X_DEVICE_ADDR
        );
    #endif

    private:
        bool                       initialized                  = false;
        float                      gasScore;
        float                      humidityScore;
        float                      airQualityScore;
        float                      gasReference;
        uint8_t                    fields;
        uint8_t                    getGasReferenceCount         = 0;
        uint8_t                    deviceAddress                = HMS_BME68X_DEVICE_ADDR;
        struct bme68x_dev          bme68XDev;
        struct bme68x_data         sensorData;
        struct bme68x_conf         sensorConf;
        struct bme68x_heatr_conf   heaterConf;

        #if defined(HMS_BME68X_PLATFORM_ARDUINO)
            TwoWire *bme68x_wire = NULL;
        #elif defined(HMS_BME68X_PLATFORM_ESP_IDF)
            i2c_port_t bme68x_i2c_port;
        #elif defined(HMS_BME68X_PLATFORM_ZEPHYR)
            struct device *bme68x_i2c_dev;
        #elif defined(HMS_BME68X_PLATFORM_STM32_HAL)
            static I2C_HandleTypeDef *bme68x_hi2c;
        #endif

                
        HMS_BME68X_StatusTypeDef init();

        static void bme68xDelayUS(uint32_t period, void *intf_ptr);
        static BME68X_INTF_RET_TYPE bme68x_i2c_read( uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr);
        static BME68X_INTF_RET_TYPE bme68x_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr);
};


#endif // HMS_BME68X_DRIVER_H