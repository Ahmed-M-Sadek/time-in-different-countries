#ifndef LCD_MODULE
#define LCD_MODULE

#define Ports

#include "stdint.h"
#include <stdint.h>
#include <stdbool.h>

#include "FreeRTOS.h"

#include "inc\hw_ssi.h"
#include "inc\hw_types.h"
#include "inc\tm4c123gh6pm.h"
#include "driverlib\pin_map.h"
#include "inc\hw_memmap.h"
#include "driverlib\ssi.h"
#include "driverlib\gpio.h"
#include "driverlib\sysctl.h"
#include "driverlib\debug.h"

#define LCD_COMMAND_CLEAR 0x1               // Clear display screen
#define LCD_COMMAND_RETURN_CURSOR 0x2       // Return cursor home
#define LCD_COMMAND_INC_CURSOR 0x6          // Increment cursor (shift cursor to right)
#define LCD_COMMAND_DISPLAY_ON 0xF          // Display on, cursor blinking
#define LCD_COMMAND_DISPLAY_ON_NOBLINK 0xC  // Display on, no cursor blinking
#define LCD_COMMAND_CURSOR_FIRST_LINE 0x80  // Force cursor to beginning of 1st line
#define LCD_COMMAND_CURSOR_SECOND_LINE 0xC0 // Force cursor to beginning of 2nd line
#define LCD_COMMAND_8_BIT 0x38              // 2 lines and 5x7 character (8-bit data, D0 to D7)
#define LCD_COMMAND_WAKEUP 0x30             // wakeup command

#define ALLDATA_8	0xFF

#define LCD_CTRL_ZERO 0
#define LCD_RS 0x80
#define LCD_RW 0x40
#define LCD_EN 0x20

#define LCD_CTRL_CLK SYSCTL_RCGCGPIO_R0
#define LCD_DATA_CLK SYSCTL_RCGCGPIO_R1
#define LCD_DATA_BASE GPIO_PORTB_BASE
#define LCD_CTRL_BASE GPIO_PORTA_BASE

extern void LCD_init(void);
extern void LCD_print(char *word);
extern void LCD_clear(void);
extern void LCD_secondLine(void);
extern bool UART_Data_Ready(void);

#endif //LCD_MODULE