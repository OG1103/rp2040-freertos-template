#include "FreeRTOS.h"
#include "task.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "bmp280_driver.h"
#include "tilt_sensor.h"
#include "motor_control.h"
#include "lcd_driver.h"
#include <stdio.h>
#include <string.h>

#define ANALOG_SENSOR1_PIN 0  // GPIO26 -> ADC0
#define ANALOG_SENSOR2_PIN 1  // GPIO27 -> ADC1
#define BUZZER_PIN 17

float current_speed = 255;       // Variable to track speed
float tire_pressure = 15.00;    // Example starting tire pressure
bool is_emergency = false;      // Flag for emergency status
char emergency_message[32] = ""; // Buffer for LCD emergency message

void sensor_task(void *pvParameters) {
    bmp280_init();
    tilt_sensor_init();
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);

    while (true) {
        float temperature = bmp280_read_temperature();
        float pressure = bmp280_read_pressure();
        tire_pressure = pressure; // Update global variable

        // Debug print for raw values
        printf("Temperature: %.2f °C\n", temperature);
        printf("Pressure: %.2f hPa\n", pressure);
        printf("Speed: %.1f km/h\n", current_speed);

        const char *tilt_state = tilt_sensor_get_state();
        printf("Tilt Sensor State: %s\n", tilt_state);

        // Reset emergency state and message
        is_emergency = false;
        memset(emergency_message, 0, sizeof(emergency_message));

        // Check conditions for temperature, pressure, speed, and tilt
        if (pressure > 21.2) {
            is_emergency = true;
            strcpy(emergency_message, "Critical High Pressure!");
        } else if (pressure < 13 && pressure > 0) {
            is_emergency = true;
            strcpy(emergency_message, "Critical Low Pressure!");
        } else if (pressure > 16.95 && current_speed >500) {
            is_emergency = true;
            strcpy(emergency_message, "High Speed + Pressure!");
        } else if (temperature > 106 && pressure > 16.95) {
            is_emergency = true;
            strcpy(emergency_message, "High Temp + Pressure!");
        } else if (pressure < 16.7 &&
                   (strcmp(tilt_state, "Tilt Right") == 0 || strcmp(tilt_state, "Tilt Left") == 0)) {
            is_emergency = true;
            strcpy(emergency_message, "Low Pressure + Tilt!");
        }

        // Handle emergency actions
        if (is_emergency) {
            gpio_put(BUZZER_PIN, 1); // Turn buzzer ON
            printf("Emergency: %s\n", emergency_message);
        } else {
            gpio_put(BUZZER_PIN, 0); // Turn buzzer OFF
        }

        vTaskDelay(pdMS_TO_TICKS(2000)); // Run every 2 seconds
    }
}

// Task to display data on the LCD
void lcd_task(void *pvParameters) {
    lcd_init();
    lcd_clear();
    while (true) {
        if (is_emergency) {
            // Emergency state: Display only the emergency message
            lcd_clear(); // Clear the display for the emergency message
            lcd_set_cursor(0, 0);
            lcd_write_string("EMERGENCY:");
            lcd_set_cursor(1, 0);
            lcd_write_string(emergency_message); // Show the emergency message
        } else {
            lcd_clear();
            // Normal state: Display speed, pressure, and temperature
            char line1[17];
            char line2[17];

            // Format the first line: Speed and Pressure
            snprintf(line1, sizeof(line1), "S:%.1f P:%.2f", current_speed, bmp280_read_pressure());

            // Format the second line: Temperature
            snprintf(line2, sizeof(line2), "T:%.1fC", bmp280_read_temperature());

            // Display formatted lines
            lcd_set_cursor(0, 0);
            lcd_write_string(line1);
            lcd_set_cursor(1, 0);
            lcd_write_string(line2);
        }

        vTaskDelay(pdMS_TO_TICKS(1000)); // Update every second
    }
}

// Task to change speed every 5 seconds
void speed_task(void *pvParameters) {
    int speeds[] = {50, 100, 150, 200, 255}; // Speed levels
    int current_index = 0;

    while (true) {
        // Set the current speed
        current_speed = (float)speeds[current_index]; // Update global speed variable

        printf("Speed changed to: %d\n", speeds[current_index]);

        // Move to the next speed level
        current_index = (current_index + 1) % (sizeof(speeds) / sizeof(speeds[0]));

        vTaskDelay(pdMS_TO_TICKS(5000)); // Wait 5 seconds before changing speed
    }
}

int main() {
    stdio_init_all();
    motor_init();
    motor_set_direction(MOTOR_FORWARD);
    motor_set_speed(255);
    // Create FreeRTOS tasks
    xTaskCreate(sensor_task, "Sensor_Task", 1024, NULL, 1, NULL);
    xTaskCreate(lcd_task, "LCD_Task", 1024, NULL, 1, NULL);
    //xTaskCreate(speed_task, "Speed_Task", 1024, NULL, 1, NULL);

    // Start the FreeRTOS scheduler
    vTaskStartScheduler();

    // Main loop (should never be reached as scheduler takes over)
    while (1) {
    }

    return 0;
}
