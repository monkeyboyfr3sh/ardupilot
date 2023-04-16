#include "Copter.h"

#ifdef USERHOOK_INIT
static void setup_uart(AP_HAL::UARTDriver *uart, const char *name)
{
    if (uart == nullptr) {
        // that UART doesn't exist on this platform
        return;
    }
    uart->begin(57600);
}
void Copter::userhook_init()
{
    // put your initialisation code here
    // this will be called once at start-up
    // hal.scheduler->delay(1000); //Ensure that the uartA can be initialized
    // setup_uart(hal.serial(0), "SERIAL0");  // console
    // setup_uart(hal.serial(1), "SERIAL1");  // telemetry 1
    // setup_uart(hal.serial(2), "SERIAL2");  // telemetry 2
    // setup_uart(hal.serial(3), "SERIAL3");  // 1st GPS
    // setup_uart(hal.serial(4), "SERIAL4");  // 2nd GPS
    setup_uart(hal.serial(5), "SERIAL5");  // python coms 
}
#endif

#ifdef USERHOOK_FASTLOOP
void Copter::userhook_FastLoop()
{
    // put your 100Hz code here
}
#endif

#ifdef USERHOOK_50HZLOOP
void Copter::userhook_50Hz()
{
    // put your 50Hz code here
}
#endif

#ifdef USERHOOK_MEDIUMLOOP
void Copter::userhook_MediumLoop()
{
    // put your 10Hz code here
}
#endif

#ifdef USERHOOK_SLOWLOOP
void Copter::userhook_SlowLoop()
{
    // put your 3.3Hz code here
    hal.serial(5)->printf("Secret message!");
    uint8_t buffer[128];
    int bytecount = hal.serial(5)->read(&buffer[0],128);
    buffer[bytecount] = 0;
    hal.serial(0)->printf("Received: '%s'\n",buffer);
}
#endif

#ifdef USERHOOK_SUPERSLOWLOOP
void Copter::userhook_SuperSlowLoop()
{
    // put your 1Hz code here
}
#endif

#ifdef USERHOOK_AUXSWITCH
void Copter::userhook_auxSwitch1(const RC_Channel::AuxSwitchPos ch_flag)
{
    // put your aux switch #1 handler here (CHx_OPT = 47)
}

void Copter::userhook_auxSwitch2(const RC_Channel::AuxSwitchPos ch_flag)
{
    // put your aux switch #2 handler here (CHx_OPT = 48)
}

void Copter::userhook_auxSwitch3(const RC_Channel::AuxSwitchPos ch_flag)
{
    // put your aux switch #3 handler here (CHx_OPT = 49)
}
#endif
