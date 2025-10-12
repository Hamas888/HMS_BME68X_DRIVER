#include "HMS_BME68X_DRIVER.h"

#if defined(HMS_BME68X_LOGGER_ENABLED)
  #include "ChronoLog.h"
  ChronoLogger bmeLogger("HMS_BME68X", CHRONOLOG_LEVEL_DEBUG);
#endif



HMS_BME68X::HMS_BME68X() {
    // Constructor
}

HMS_BME68X::~HMS_BME68X() {
    // Destructor
}   

float HMS_BME68X::getGasScore() {
	gasScore = (
        0.75 / (HMS_BME68X_GAS_UPPER_LIMIT - HMS_BME68X_GAS_LOWER_LIMIT) * gasReference - (
            HMS_BME68X_GAS_LOWER_LIMIT * (
            0.75 / (HMS_BME68X_GAS_UPPER_LIMIT - HMS_BME68X_GAS_LOWER_LIMIT)
        ))
    ) * 100.00;

	if (gasScore > 75)
		gasScore = 75; // Sometimes gas readings can go outside of expected scale maximum
	if (gasScore < 0)
		gasScore = 0; // Sometimes gas readings can go outside of expected scale minimum

    #ifdef HMS_BME68X_LOGGER_ENABLED
        bmeLogger.debug("Gas Resistance: %.2f Ohms, Gas Score: %.2f %%", sensorData.gas_resistance, gasScore);
    #endif
	return gasScore;
}

float HMS_BME68X::getAirQuality() {
	airQualityScore = (
        100 - (getHumidityScore()
        + getGasScore())
    ) * 5;

	if ((getGasReferenceCount++) % 5 == 0)                  // If 5 measurements passed, recalculate the gas reference.
		getGasReference();

    #ifdef HMS_BME68X_LOGGER_ENABLED
        bmeLogger.debug("Air Quality Score: %.2f %%", airQualityScore);
    #endif
	return airQualityScore;
}

float HMS_BME68X::getGasReference() {
    int readings = 10;
	for (int i = 1; i <= readings; i++) {                                                   // read gas for 10 x 0.150mS = 1.5secs
		read(&sensorData);
		gasReference += sensorData.gas_resistance;
	}
	gasReference = gasReference / readings;

    #ifdef HMS_BME68X_LOGGER_ENABLED
        bmeLogger.debug("Gas Reference: %.2f Ohms", gasReference);
    #endif
    return gasReference;
}

float HMS_BME68X::getHumidityScore() {
	if (sensorData.humidity >= 38 && sensorData.humidity <= 42)                           // Humidity +/-5% around optimum
		humidityScore = 0.25 * 100;
	else {                                                                                  // Humidity is sub-optimal
		if (sensorData.humidity < 38)
			humidityScore = (
                0.25 / HMS_BME68X_HUMIDITY_REFERENCE * sensorData.humidity * 100
            );
		else {
			humidityScore = ((   
                    -0.25 / (100 - HMS_BME68X_HUMIDITY_REFERENCE)
                    * sensorData.humidity
                ) + 0.416666
            ) * 100;
		}
	}

    #ifdef HMS_BME68X_LOGGER_ENABLED
        bmeLogger.debug("Humidity: %.2f %%rH, Humidity Score: %.2f %%", sensorData.humidity, humidityScore);
    #endif

	return humidityScore;
}

void HMS_BME68X::read(struct bme68x_data *data) {
    if(!data) return;

	bme68x_set_op_mode(BME68X_FORCED_MODE, &bme68XDev);

	//Calculate delay period in microseconds
	uint32_t delayPeriod = bme68x_get_meas_dur(
        BME68X_FORCED_MODE, &sensorConf, &bme68XDev
    ) + (heaterConf.heatr_dur * 1000);

	bme68XDev.delay_us(delayPeriod, bme68XDev.intf_ptr);

	bme68x_get_data(BME68X_FORCED_MODE, data, &fields, &bme68XDev);

    sensorData = *data;
    #ifdef HMS_BME68X_LOGGER_ENABLED
        bmeLogger.debug(
            "T: %.2f C, P: %.2f hPa, H: %.2f %%rH, G: %.2f Ohms",
            sensorData.temperature,
            sensorData.pressure / 100.0,
            sensorData.humidity,
            sensorData.gas_resistance
        );
    #endif
}

#if defined(HMS_BME68X_PLATFORM_ARDUINO)
HMS_BME68X_StatusTypeDef HMS_BME68X::begin(TwoWire *theWire, uint8_t addr) {
    // Placeholder implementation for Arduino
    return HMS_BME68X_OK;
}
#elif defined(HMS_BME68X_PLATFORM_ESP_IDF)
HMS_BME68X_StatusTypeDef HMS_BME68X::begin(i2c_port_t i2c_port, uint8_t addr) {
    // Placeholder implementation for ESP-IDF
    return HMS_BME68X_OK;
}
#elif defined(HMS_BME68X_PLATFORM_ZEPHYR)
HMS_BME68X_StatusTypeDef HMS_BME68X::begin(const struct device *i2c_dev, uint8_t addr) {
    // Placeholder implementation for Zephyr
    return HMS_BME68X_OK;
}
#elif defined(HMS_BME68X_PLATFORM_STM32_HAL)
HMS_BME68X_StatusTypeDef HMS_BME68X::begin(I2C_HandleTypeDef *hi2c, uint8_t addr) {
    if (hi2c == NULL) {
        #ifdef HMS_BME68X_LOGGER_ENABLED
            bmeLogger.error("I2C handle is NULL");
        #endif
        return HMS_BME68X_ERROR;
    }

    bme68x_hi2c = hi2c;
    deviceAddress = addr;

    HAL_StatusTypeDef result;
    result = HAL_I2C_IsDeviceReady(bme68x_hi2c, deviceAddress << 1, 3, 100);

    if (result != HAL_OK) {
        #ifdef HMS_BME68X_LOGGER_ENABLED
            bmeLogger.error("Device not found at address 0x%02X", deviceAddress);
        #endif
        return HMS_BME68X_NOT_FOUND;
    }
    #ifdef HMS_BME68X_LOGGER_ENABLED
        bmeLogger.info("Device found at address 0x%02X", deviceAddress);
    #endif

    return init();
    return HMS_BME68X_OK;
}

BME68X_INTF_RET_TYPE HMS_BME68X::readRegister(
    uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr
) {
	uint8_t devAddr = *(uint8_t*) intf_ptr;

	if (
        HAL_I2C_Mem_Read(bme68x_hi2c, (uint16_t) ((devAddr << 1) | 0x1), reg_addr,
		I2C_MEMADD_SIZE_8BIT, reg_data, len, 15) == HAL_OK
    ) return 0;

    #ifdef HMS_BME68X_LOGGER_ENABLED
        bmeLogger.error("I2C Read Error at reg 0x%02X", reg_addr);
    #endif

	return 1;
}

BME68X_INTF_RET_TYPE HMS_BME68X::writeRegister(
    uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr 
) {
    uint8_t devAddr = *(uint8_t*) intf_ptr;

    if (
        HAL_I2C_Mem_Write(bme68x_hi2c, (uint16_t) (devAddr << 1), reg_addr,
        I2C_MEMADD_SIZE_8BIT, (uint8_t*) reg_data, len, 100) == HAL_OK
    ) return 0;

    #ifdef HMS_BME68X_LOGGER_ENABLED
        bmeLogger.error("I2C Write Error at reg 0x%02X", reg_addr);
    #endif
    return 1;
}
#endif

HMS_BME68X_StatusTypeDef HMS_BME68X::init() {
    if (initialized) {
        return HMS_BME68X_OK;                                           // Already initialized
    }

    bme68XDev.read      = readRegister;
    bme68XDev.write     = writeRegister;
    bme68XDev.intf      = BME68X_I2C_INTF;
    bme68XDev.delay_us  = bme68xDelayUS;
    bme68XDev.intf_ptr  = &deviceAddress;                                // Pass device address as interface pointer
    bme68XDev.amb_temp  = HMS_BME68X_AMBIENT_TEMP_DEFAULT;               // Set default ambient temperature

    if(bme68x_init(&bme68XDev) != BME68X_OK) {
        #ifdef HMS_BME68X_LOGGER_ENABLED
            bmeLogger.error("Failed to initialize BME68X sensor");
        #endif
        return HMS_BME68X_ERROR;
    }

    sensorConf.filter  = BME68X_FILTER_SIZE_3;
	sensorConf.odr     = BME68X_ODR_NONE;
	sensorConf.os_hum  = BME68X_OS_2X;
	sensorConf.os_pres = BME68X_OS_4X;
	sensorConf.os_temp = BME68X_OS_8X;

    if(bme68x_set_conf(&sensorConf, &bme68XDev) != BME68X_OK) {
        #ifdef HMS_BME68X_LOGGER_ENABLED
            bmeLogger.error("Failed to set sensor configuration");
        #endif
        return HMS_BME68X_ERROR;
    }

    heaterConf.enable       = BME68X_ENABLE;
	heaterConf.heatr_temp   = HMS_BME68X_HEATER_TEMP_DEFAULT;
	heaterConf.heatr_dur    = HMS_BME68X_HEATER_DUR_DEFAULT;

    if(bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heaterConf, &bme68XDev)  != BME68X_OK) {
        #ifdef HMS_BME68X_LOGGER_ENABLED
            bmeLogger.error("Failed to set heater configuration");
        #endif
        return HMS_BME68X_ERROR;
    }

    initialized = true;
    #ifdef HMS_BME68X_LOGGER_ENABLED
        bmeLogger.info("BME68X sensor initialized successfully");
    #endif
    return HMS_BME68X_OK;
}

void HMS_BME68X::bme68xDelayUS(uint32_t period, void *intf_ptr) {
    (void)intf_ptr;  // Suppress unused parameter warning
    #if defined(HMS_BME68X_PLATFORM_ARDUINO)
        delay(period / 1000);
    #elif defined(HMS_BME68X_PLATFORM_ESP_IDF)
        vTaskDelay((period / portTICK_PERIOD_MS) / 1000);
    #elif defined(HMS_BME68X_PLATFORM_ZEPHYR)
        k_msleep(period / 1000);
    #elif defined(HMS_BME68X_PLATFORM_STM32_HAL)
        HAL_Delay(period / 1000);
    #endif
}