/*
  simple test of UART interfaces
 */

#include <AP_HAL/AP_HAL.h>
#include <Companion_IF/Companion_IF.h>

const AP_HAL::HAL& hal = AP_HAL::get_HAL();

#if HAL_OS_POSIX_IO
#include <stdio.h>
#endif

void setup();
void loop();

/*
  setup one UART at 57600
 */
static void setup_uart(AP_HAL::UARTDriver *uart, const char *name)
{
    if (uart == nullptr) {
        // that UART doesn't exist on this platform
        return;
    }
    uart->begin(115200);
}

void setup(void)
{
    hal.scheduler->delay(1000); //Ensure that the uartA can be initialized

    setup_uart(hal.serial(0), "SERIAL0");  // console
    setup_uart(hal.serial(1), "SERIAL1");  // telemetry 1
    setup_uart(hal.serial(2), "SERIAL2");  // telemetry 2
    setup_uart(hal.serial(3), "SERIAL3");  // 1st GPS
    setup_uart(hal.serial(4), "SERIAL4");  // 2nd GPS
    setup_uart(hal.serial(5), "SERIAL5");  // python coms
}

int print_buffer(const char* buff, size_t max_len) {
    const char * string_start = &(buff[1]);
    size_t len = strnlen(string_start, max_len); // Check length up to 100 characters
    hal.serial(0)->printf("%s", string_start); // Use precision to print up to len characters
    return len;
}

void loop(void)
{
    const uint16_t connect_time = 100;
    static Companion_IF companion_if = Companion_IF(hal.serial(5),connect_time);
    static uint8_t rx_buff[128];
    static int rx_count = 0;

    // Need to connect first
    if ( !companion_if.connected ){
        hal.serial(0)->printf("error connecting!!\r\n");
        companion_if.connect_to_companion(connect_time);
    } 
    // Connected and ready to roll    
    else {
        CompanionCommandType command = companion_if.poll_for_command(rx_buff,sizeof(rx_buff));
        if (command !=  CompanionCommandType::no_cmd){
            // Update rx count
            rx_count++;
            companion_if._companion_uart->printf("AP rx_count %d... ", rx_count);
            hal.serial(0)->printf("Got command: ");

            // Handle command return
            switch (command)
            {
            case CompanionCommandType::string_cmd:{
                hal.serial(0)->printf("[STR] ");
                // Print the stirng
                print_buffer( (const char*)rx_buff, sizeof(rx_buff));
                hal.serial(0)->printf("\r\n");
                break;
            }
            case CompanionCommandType::ack_cmd:{
                // Print an ack
                hal.serial(0)->printf("[ACK]\r\n");
                break;
            }
            
            default:
                break;
            }
        }
    }
}

AP_HAL_MAIN();
