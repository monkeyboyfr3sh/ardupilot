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
    uart->begin(57600);
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

uint8_t buffer[128];
void loop(void)
{
    static Companion_IF my_companion = Companion_IF(hal.serial(5));

    // int bytecount = hal.serial(5)->read(&buffer[0],128);
    // if(bytecount>0){
    //     hal.serial(5)->printf("Secret message!");
    
    //     buffer[bytecount] = 0;
    //     // hal.serial(0)->printf("Received: '%s'\n",buffer);
    //     hal.scheduler->delay(100);
    // }
    my_companion.custom_loop_action();

    hal.scheduler->delay(100);
}

AP_HAL_MAIN();
