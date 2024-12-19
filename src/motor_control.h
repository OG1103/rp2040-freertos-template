#include "pico/stdlib.h"

#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

// Motor control GPIO pins
#define MOTOR_IN1 18  // H-Bridge IN1
#define MOTOR_IN2 19  // H-Bridge IN2
#define MOTOR_ENABLE 25  // Optional: PWM for speed control

// Motor direction states
typedef enum {
    MOTOR_STOP,
    MOTOR_FORWARD,
    MOTOR_BACKWARD
} MotorDirection;

// Function prototypes
void motor_init(void);
void motor_set_direction(MotorDirection direction);
void motor_set_speed(uint8_t speed); // Speed: 0 to 255
void motor_stop(void);

#endif // MOTOR_CONTROL_H
