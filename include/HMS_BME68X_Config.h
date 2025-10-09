   /*
 ====================================================================================================
 * File:        HMS_BME68X_Config.h
 * Author:      Hamas Saeed
 * Version:     Rev_1.0.0
 * Date:        Oct 9 2025
 * Brief:       This Package Provide BME68X Sensor Driver Configuration.
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

#ifndef HMS_BME68X_CONFIG_H
#define HMS_BME68X_CONFIG_H

/*
  ┌─────────────────────────────────────────────────────────────────────┐
  │ Note:     Enable only if ChronoLog is included                      │
  │ Requires: ChronoLog library → https://github.com/Hamas888/ChronoLog │
  └─────────────────────────────────────────────────────────────────────┘
*/
#define HMS_BME68X_DEBUG_ENABLED           0                               // Enable debug messages (1=enabled, 0=disabled)


#define HMS_BME68X_DEVICE_NAME             "BME68X"                       // Device Name
#define HMS_BME68X_DEVICE_ADDR             0x76                           // BME68X default I2C address


#endif // HMS_BME68X_CONFIG_H