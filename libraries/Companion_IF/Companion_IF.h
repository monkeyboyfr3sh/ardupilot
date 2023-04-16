#pragma once
#include <AP_HAL/AP_HAL.h>

class Companion_IF {
public:
    // Constructor
    Companion_IF(AP_HAL::UARTDriver* companion_uart);
    AP_HAL::UARTDriver* _companion_uart;
    void custom_loop_action(void);
};

