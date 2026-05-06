# HMS_BME68X_DRIVER/CMakeLists.txt

set(HMS_BME68X_DRIVER_VERSION 1.0.2)

# Check if we're building with Zephyr
if(DEFINED ZEPHYR_BASE)
    zephyr_include_directories(include)
    zephyr_include_directories(BME68x_SensorAPI)
    zephyr_library_sources(src/HMS_BME68X_DRIVER.cpp)
    zephyr_library_sources(BME68x_SensorAPI/bme68x.c)

# Check if we're building with ESP-IDF
elseif(DEFINED ESP_PLATFORM OR DEFINED IDF_VER OR DEFINED ENV{IDF_PATH})
    idf_component_register(
        SRCS "src/HMS_BME68X_DRIVER.cpp"
        SRCS "BME68x_SensorAPI/bme68x.c"
        INCLUDE_DIRS "include"
        INCLUDE_DIRS "BME68x_SensorAPI"
        REQUIRES "driver"
    )
    
# STM32 / generic CMake project
else()
    add_library(HMS_BME68X_DRIVER INTERFACE)
    target_include_directories(HMS_BME68X_DRIVER INTERFACE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/BME68x_SensorAPI
    )
    target_compile_features(HMS_BME68X_DRIVER INTERFACE cxx_std_17)
endif()