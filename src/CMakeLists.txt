add_executable(${ProjectName}
    main.c
    bmp280_driver.c
    lcd_driver.c
    motor_control.c
    tilt_sensor.c
)

target_include_directories(${ProjectName} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(${ProjectName} 
    pico_stdlib 
    FreeRTOS-Kernel-Heap4 
    hardware_i2c  # Add I2C library
    hardware_adc  # Add ADC library
    hardware_pwm
    )

pico_add_extra_outputs(${ProjectName})