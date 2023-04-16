#include "Companion_IF.h"

extern const AP_HAL::HAL& hal;

Companion_IF::Companion_IF(AP_HAL::UARTDriver* companion_uart)
{
    _companion_uart = companion_uart;
}

void Companion_IF::custom_loop_action(void)
{
    _companion_uart->printf("Secret message!");
}
