#ifndef BMP280_DRIVER_H
#define BMP280_DRIVER_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"

// BMP280 Default I2C Address
#define BMP280_I2C_ADDR 0x76

// I2C Pins for BMP280
#define I2C_SDA_PIN 12  // GPIO12 for SDA
#define I2C_SCL_PIN 13  // GPIO13 for SCL

// BMP280 Driver Functions
void bmp280_init();
float bmp280_read_temperature();
float bmp280_read_pressure();

#endif // BMP280_DRIVER_H