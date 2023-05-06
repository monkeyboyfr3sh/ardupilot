#include "Companion_IF.h"
#include <AP_Scheduler/AP_Scheduler.h>

extern const AP_HAL::HAL& hal;
static AP_Scheduler scheduler;

// List of commands
const char * command_strings[] = {
    "AP String 1",
    "AP String 2",
};

Companion_IF::Companion_IF(AP_HAL::UARTDriver* companion_uart, uint16_t timeout_s)
{
    // Assign the companion uart
    _companion_uart = companion_uart;
    connected = false;

    // Now try connecting to companion
    hal.serial(0)->write("Connecting to companion...");
    CompanionErrType err = connect_to_companion(timeout_s);
    if (err != CompanionErrType::OK){
        hal.serial(0)->write("Failed!\r\n");
        connected = false;
    } else {
        hal.serial(0)->write("Success!\r\n");
        connected = true;
    }
}

void Companion_IF::custom_loop_action(void)
{
    _companion_uart->printf("Secret message!");
}

CompanionErrType Companion_IF::connect_to_companion(uint16_t timeout_s)
{
    const char* msg = "Hello from AP";
    const uint16_t delay_ms = 100;
    const int timeout_ms = timeout_s*1000;
    uint8_t buffer[128];

    // Stay here writing a message then waiting for a response
    int bytecount = 0;
    int num_tries = 0;
    do {
        _companion_uart->write(msg);
        bytecount = _companion_uart->read(&buffer[0],128);
        hal.scheduler->delay(delay_ms);
    } while( (bytecount<=0) && (num_tries++<=(timeout_ms/delay_ms)) );

    // If we got a byte count then all good
    if (bytecount>0){
        return CompanionErrType::OK;
    } else {
        return CompanionErrType::INIT_FAIL;
    }

}

CompanionCommandType Companion_IF::lookup_command(const char * cmd_buff)
{
    CompanionCommandType ret = CompanionCommandType::bad_cmd;

    uint8_t command_code = *cmd_buff;
    switch (command_code)
    {
    // String based command
    case (uint8_t)CompanionCommandCode::string_cmd_code:
        ret = CompanionCommandType::string_cmd;
        break;
    
    // Simple hi command
    case (uint8_t)CompanionCommandCode::hi_cmd_code:
        ret = CompanionCommandType::hello_cmd;
        break;
    case (uint8_t)CompanionCommandCode::ack_cmd_code:
        ret = CompanionCommandType::ack_cmd;
        break;


    // Unknown code
    default:
        ret = CompanionCommandType::bad_cmd;
        break;
    }

    return ret;
}

CompanionCommandType Companion_IF::poll_for_command(uint8_t * rx_buff, uint16_t rx_buff_len)
{
    // First read the buffer
    int bytecount = _companion_uart->read(rx_buff,rx_buff_len);

    // Got a command to lookup
    if (bytecount>0){
        return lookup_command( (const char *)rx_buff );
    } 
    // No data
    else {
        return CompanionCommandType::no_cmd;
    }
}