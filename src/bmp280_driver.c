#include "bmp280_driver.h"

void bmp280_init() {
    // Configure BMP280 control and config registers
    uint8_t config[] = {
        0xF4, 0x27,  // Control: Normal mode, temp/pressure oversampling x1
        0xF5, 0xA0   // Config: Standby 1000ms, filter off
    };

    // Initialize I2C pins
    i2c_init(i2c_default, 100 * 1000);  // 100 kHz
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    // Write configuration to BMP280
    i2c_write_blocking(i2c_default, BMP280_I2C_ADDR, config, 2, false);
}

float bmp280_read_temperature() {
    uint8_t reg = 0xFA;  // Temperature MSB register
    uint8_t buffer[3];

    // Read 3 bytes of temperature data
    i2c_write_blocking(i2c_default, BMP280_I2C_ADDR, &reg, 1, true);
    i2c_read_blocking(i2c_default, BMP280_I2C_ADDR, buffer, 3, false);

    // Combine the 3 bytes into a raw temperature value
    int32_t temp_raw = (buffer[0] << 12) | (buffer[1] << 4) | (buffer[2] >> 4);

    // Convert to Celsius (simplified formula)
    return temp_raw / 5120.0;
}

float bmp280_read_pressure() {
    uint8_t reg = 0xF7;  // Pressure MSB register
    uint8_t buffer[3];
    

    // Read 3 bytes of pressure data
    i2c_write_blocking(i2c_default, BMP280_I2C_ADDR, &reg, 1, true);
    i2c_read_blocking(i2c_default, BMP280_I2C_ADDR, buffer, 3, false);

    // Combine the 3 bytes into a raw pressure value
    int32_t pressure_raw = (buffer[0] << 12) | (buffer[1] << 4) | (buffer[2] >> 4);

    // Convert to hPa (simplified formula)
    return pressure_raw / 25600.0;
}