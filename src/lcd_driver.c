#include "lcd_driver.h"
#include "pico/stdlib.h"
#include <string.h>
#include <stdio.h>

void pulse_enable() {
    gpio_put(LCD_EN, 1);
    sleep_us(1);
    gpio_put(LCD_EN, 0);
    sleep_us(50);
}

void lcd_send_nibble(uint8_t nibble) {
    gpio_put(LCD_D4, (nibble >> 0) & 1);
    gpio_put(LCD_D5, (nibble >> 1) & 1);
    gpio_put(LCD_D6, (nibble >> 2) & 1);
    gpio_put(LCD_D7, (nibble >> 3) & 1);
    pulse_enable();
}

void lcd_command(uint8_t cmd) {
    gpio_put(LCD_RS, 0);  // Command mode
    lcd_send_nibble(cmd >> 4);
    lcd_send_nibble(cmd);
    sleep_ms(2);
}

void lcd_write_data(uint8_t data) {
    gpio_put(LCD_RS, 1);  // Data mode
    lcd_send_nibble(data >> 4);
    lcd_send_nibble(data);
    sleep_us(50);
}

void lcd_clear() {
    lcd_command(0x01);  // Clear display command
    sleep_ms(2);
}

void lcd_set_cursor(uint8_t row, uint8_t col) {
    uint8_t address = col + (row ? 0x40 : 0x00);
    lcd_command(0x80 | address);
}

void lcd_write_string(const char *str) {
    while (*str) {
        lcd_write_data(*str++);
    }
}

void lcd_init() {
    gpio_init(LCD_RS);
    gpio_init(LCD_EN);
    gpio_init(LCD_D4);
    gpio_init(LCD_D5);
    gpio_init(LCD_D6);
    gpio_init(LCD_D7);

    gpio_set_dir(LCD_RS, GPIO_OUT);
    gpio_set_dir(LCD_EN, GPIO_OUT);
    gpio_set_dir(LCD_D4, GPIO_OUT);
    gpio_set_dir(LCD_D5, GPIO_OUT);
    gpio_set_dir(LCD_D6, GPIO_OUT);
    gpio_set_dir(LCD_D7, GPIO_OUT);

    // Initialize LCD
    sleep_ms(50);  // Wait for LCD to power up
    lcd_send_nibble(0x03);
    sleep_ms(5);
    lcd_send_nibble(0x03);
    sleep_us(150);
    lcd_send_nibble(0x03);
    lcd_send_nibble(0x02);

    lcd_command(0x28);  // 4-bit mode, 2-line display
    lcd_command(0x0C);  // Display ON, Cursor OFF
    lcd_clear();
}
