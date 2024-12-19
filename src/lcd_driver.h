#ifndef LCD_DRIVER_H
#define LCD_DRIVER_H

#include "pico/stdlib.h"

// LCD pin definitions
#define LCD_RS 20
#define LCD_EN 21
#define LCD_D4 5
#define LCD_D5 7
#define LCD_D6 4
#define LCD_D7 26

// Function prototypes
void lcd_init();
void lcd_clear();
void lcd_set_cursor(uint8_t row, uint8_t col);
void lcd_write_string(const char *str);
void lcd_command(uint8_t cmd);
void lcd_write_data(uint8_t data);

#endif
