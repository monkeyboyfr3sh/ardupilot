#pragma once
#include <AP_HAL/AP_HAL.h>
#include <stdarg.h>

#define COMPANION_INIT_MESSAGE "Companion Link MSG!"

enum class CompanionErrType {
    OK,
    INIT_FAIL
};

enum class CompanionCommandCode {
    ack_cmd_code = 0x06,
    string_cmd_code = 0x07,
    hi_cmd_code     = 0x0A,
};

enum class CompanionCommandType {
    no_cmd,
    bad_cmd,
    hello_cmd,
    string_cmd,
    ack_cmd,
};

class Companion_IF {
public:
    // Constructor
    Companion_IF(AP_HAL::UARTDriver* companion_uart, uint16_t timeout_s);

    CompanionErrType connect_to_companion(uint16_t timeout_s);
    void debug_printing(const char* format, ...);
    void print_buffer(const char *buffer);
    CompanionCommandType lookup_command(const char *cmd_buff);
    CompanionCommandType poll_for_command(uint8_t * rx_buff, uint16_t rx_buff_len);
    void custom_loop_action(void);

    AP_HAL::UARTDriver* _companion_uart;
    bool connected;
};

