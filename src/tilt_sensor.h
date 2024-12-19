#ifndef TILT_SENSOR_H
#define TILT_SENSOR_H

#include "pico/stdlib.h"

// GPIO pins connected to the tilt sensor
#define TILT_SENSOR_S1_PIN 15
#define TILT_SENSOR_S2_PIN 16

// Function prototypes
void tilt_sensor_init();
const char* tilt_sensor_get_state();

#endif // TILT_SENSOR_H
