#include "motor_control.h"
#include "hardware/pwm.h"

// Initialize motor GPIO pins
void motor_init(void) {
    gpio_init(MOTOR_IN1);
    gpio_set_dir(MOTOR_IN1, GPIO_OUT);

    gpio_init(MOTOR_IN2);
    gpio_set_dir(MOTOR_IN2, GPIO_OUT);

    gpio_init(MOTOR_ENABLE);
    gpio_set_dir(MOTOR_ENABLE, GPIO_OUT);

    // Set up PWM for speed control (optional)
    gpio_set_function(MOTOR_ENABLE, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(MOTOR_ENABLE);
    pwm_set_wrap(slice_num, 255);  // Set max duty cycle
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(MOTOR_ENABLE), 0);
    pwm_set_clkdiv(slice_num,4.0f);
    pwm_set_enabled(slice_num, true);
}

// Set motor direction
void motor_set_direction(MotorDirection direction) {
    switch (direction) {
        case MOTOR_FORWARD:
            gpio_put(MOTOR_IN1, 1);
            gpio_put(MOTOR_IN2, 0);
            break;
        case MOTOR_BACKWARD:
            gpio_put(MOTOR_IN1, 0);
            gpio_put(MOTOR_IN2, 1);
            break;
        case MOTOR_STOP:
        default:
            gpio_put(MOTOR_IN1, 0);
            gpio_put(MOTOR_IN2, 0);
            break;
    }
}

// Set motor speed (0 to 255)
void motor_set_speed(uint8_t speed) {
    uint slice_num = pwm_gpio_to_slice_num(MOTOR_ENABLE);
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(MOTOR_ENABLE), speed);
}

// Stop the motor
void motor_stop(void) {
    motor_set_direction(MOTOR_STOP);
    motor_set_speed(0);
}
