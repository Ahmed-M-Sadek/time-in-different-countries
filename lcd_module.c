#include "TM4C123GH6PM.h"
#include <stdint.h>

#include "stdint.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_gpio.h"

#include "lcd_module.h"

void LCD_write(unsigned char data);
void delay_ms(int32_t timeMS);

void LCD_command(unsigned char command)
{
		//Set LCD mode to command mode
    GPIOPinWrite(GPIO_PORTA_BASE, LCD_RS | LCD_RW | LCD_EN, LCD_CTRL_ZERO);
		//Enter the LCD command to data pins
    GPIOPinWrite(GPIO_PORTB_BASE, ALLDATA_8, command);
    GPIOPinWrite(GPIO_PORTA_BASE, LCD_RS | LCD_RW | LCD_EN, LCD_EN);
    delay_ms(1);
    GPIOPinWrite(GPIO_PORTA_BASE, LCD_RS | LCD_RW | LCD_EN, LCD_CTRL_ZERO);

    if (command > LCD_COMMAND_RETURN_CURSOR)
        delay_ms(1);
    else
        delay_ms(2);
}

void LCD_write(unsigned char data)
{
		//Set LCD mode to data write mode
    GPIOPinWrite(GPIO_PORTA_BASE, LCD_RS | LCD_RW | LCD_EN, LCD_RS);
		//Enter the data to be displayed to data pins
    GPIOPinWrite(GPIO_PORTB_BASE, ALLDATA_8, data);
    GPIOPinWrite(GPIO_PORTA_BASE, LCD_RS | LCD_RW | LCD_EN, LCD_EN | LCD_RS);
    delay_ms(1);
    GPIOPinWrite(GPIO_PORTA_BASE, LCD_RS | LCD_RW | LCD_EN, LCD_CTRL_ZERO);
    delay_ms(1);
}

void LCD_clear(void)
{
		//Command to clear the LCD and reset to first line
    LCD_command(LCD_COMMAND_CLEAR);
}

void LCD_secondLine(void)
{
		//Command to switch the cursor position to the second line
    LCD_command(LCD_COMMAND_CURSOR_SECOND_LINE);
}

void LCD_print(char *word)
{
		//Write the string at the current cursor position
    while (*word)
    {
        LCD_write(*word++);
    }
}

// Set the configurations for the DATA pins of the LCD
void initDataPort(void)
{
		//Enable clock to Port B
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while (!(HWREG(SYSCTL_PRGPIO) & LCD_DATA_CLK))
    {
    }
		// Configure GPIO Pins for Output.
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, ALLDATA_8);
    GPIOPinWrite(GPIO_PORTB_BASE, ALLDATA_8, 0x00);
}

// Set the configurations for the CTRL pins of the LCD
void initCtrlPort(void)
{
		//Enable clock to Port A
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while (!(HWREG(SYSCTL_PRGPIO) & LCD_CTRL_CLK))
    {
    }
		// Configure GPIO Pins for Output.
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, LCD_RS | LCD_RW | LCD_EN);
    GPIOPinWrite(GPIO_PORTA_BASE, LCD_RS | LCD_RW | LCD_EN, 0x00);
}

void LCD_start(void)
{
		// Init sequence of the LCD (as per the LCD documentation)
    delay_ms(20);
    LCD_command(LCD_COMMAND_WAKEUP);
    delay_ms(5);
    LCD_command(LCD_COMMAND_WAKEUP);
    delay_ms(1);
    LCD_command(LCD_COMMAND_WAKEUP);
    delay_ms(1);
    LCD_command(LCD_COMMAND_8_BIT);
    LCD_command(LCD_COMMAND_INC_CURSOR);
    LCD_command(LCD_COMMAND_CLEAR);
    LCD_command(LCD_COMMAND_DISPLAY_ON_NOBLINK);
}

void LCD_init(void)
{
    initDataPort();
    initCtrlPort();
    LCD_start();
}

// Delay used for initialization and sending commands and data to LCD
void delay_ms(int32_t timeMS)
{
    vTaskDelay(pdMS_TO_TICKS(timeMS));
}
