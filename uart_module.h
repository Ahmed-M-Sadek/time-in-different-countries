#ifndef UART_MODULE
#define UART_MODULE

#define PART_TM4C123GH6PM

#include <stdint.h>
#include <stdbool.h>

#include "inc\hw_ssi.h"
#include "inc\hw_types.h"
#include "inc\tm4c123gh6pm.h"
#include "inc\hw_memmap.h"
#include "inc\hw_uart.h"
#include "driverlib\pin_map.h"
#include "driverlib\uart.h"
#include "driverlib\ssi.h"
#include "driverlib\gpio.h"
#include "driverlib\sysctl.h"
#include "driverlib\debug.h"


#define BAUDRATE 115200

extern void UART_init(void);
extern char UART_receive(void);
extern void UART_receive_string (uint8_t* str);
extern void UART_send(char x);
extern void UART_send_string (uint8_t* data);

#endif //UART_MODULE