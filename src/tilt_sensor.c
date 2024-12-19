#include "tilt_sensor.h"
#include "stdio.h"

// Initialize the GPIO pins for the tilt sensor
void tilt_sensor_init() {
    gpio_init(TILT_SENSOR_S1_PIN);
    gpio_set_dir(TILT_SENSOR_S1_PIN, GPIO_IN);  // Set S1 as input
    gpio_pull_up(TILT_SENSOR_S1_PIN);           // Enable pull-up resistor

    gpio_init(TILT_SENSOR_S2_PIN);
    gpio_set_dir(TILT_SENSOR_S2_PIN, GPIO_IN);  // Set S2 as input
    gpio_pull_up(TILT_SENSOR_S2_PIN);           // Enable pull-up resistor
}

// Get the state of the tilt sensor as a string
const char* tilt_sensor_get_state() {
    bool s1 = gpio_get(TILT_SENSOR_S1_PIN);  // Read S1 signal
    bool s2 = gpio_get(TILT_SENSOR_S2_PIN);  // Read S2 signal

    printf("Debug: S1 = %d, S2 = %d\n", s1, s2);

    // Decode tilt state based on sensor outputs
    if (!s1 && !s2) {
        return "Tilt Right";        // No tilt detected
    } else if (!s1 && s2) {
        return "Upside Down";   // Tilt left detected
    } else if (s1 && !s2) {
        return "Flat";  // Tilt right detected
    } else if (s1 && s2) {
        return "Tilt Left"; // Upside-down detected
    } else {
        return "Unknown";     // Unexpected state
    }
}
