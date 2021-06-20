#include "uart_module.h"

void UART_init(void)
{// Enabling clock to UART 0 and Port A
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    
    // Configure GPIO Pins for UART mode.
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    
    //Use the internal 16MHz oscillator as the UART clock source
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_SYSTEM);
    
    //16MHz, parity none, one stop bit, 8 bits
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), BAUDRATE,(UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE |UART_CONFIG_WLEN_8));
}
char UART_receive(void)
{ 
    //Polling until data is received
    while ((UART0_FR_R&(1<<4)) != 0);
    //return the data register content
    return UART0_DR_R;
  }

void UART_send(char x)
{
	//Polling until data can be transmitted
    while ((UART0_FR_R&(1<<5)) != 0);
    //Put the data in the data register
    UART0_DR_R = x;
}
void UART_receive_string (uint8_t* str)
{
  uint8_t i=0;
  str[i] = UART_receive();
  while (str[i] != '.'){
    i++;
    str[i] = UART_receive();
  }
  str[i] = '\0';
}

void UART_send_string (uint8_t* str)
	{
  while (*str != '\0'){
    UART_send(*str);
    str++;
  }
}
bool UART_Data_Ready(void){
		//Check if there is data waiting to be recieved
		return ((UART0_FR_R&(1<<4)) == 0);
}