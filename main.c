// I*==============================================================================
// TIME IN DIFFERENT CITIES
// In this project, an LCD and a USB UART click board are connected to the Clicker 2
// for STM32 development board. This is a clock project where by default the time
// in London is displayed on the LCD. The user can select a different city via the
// keyboard and the time in the selected city is displayed every second.
// Author: Dogan Ibrahim
// Date November, 2019
// File citytimes.c
// ==============================================================================*

#include "FreeRTOS.h"
#include "TM4C123GH6PM.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "lcd_module.h"
#include "uart_module.h"

//
//  Cities and their time differences from London
//

char cities[][10] = {"London", "Paris", "Madrid", "Rome", "Athens", "Ankara", "Istanbul",
                     "Cairo", "Moscow", "Tehran"};
char timediff[] = {0, 1, 1, 1, 2, 2, 2, 2, 3, 4};

unsigned int selection;
QueueHandle_t xQueue1; //Queue1 handle to send data to LCD
QueueHandle_t xQueue2; //Queue2 handle to send data to UCONTROLLER

//
//  Define all your Task functions here
//

void Taskl(void *pvParameters);
void Task2(void *pvParameters);
void Task3(void *pvParameters);

unsigned int Read_From_Keyboard();
void ByteToStr(char ch, char b[]);

//
// 								Task 1 - Time Controller
//
// A Task responsible for incrementing the time by 1 second
// every second and making sure the time format is correct
//
void Taskl(void *pvParameters)
{
    typedef struct Message
    {
        unsigned char hours;
        unsigned char minutes;
        unsigned char seconds;
    } AMessage;
    AMessage Tim;
    xQueueReceive(xQueue2, &Tim, portMAX_DELAY); //Receive initial time
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        vTaskDelayUntil(&xLastWakeTime, 1000 / portTICK_RATE_MS);
        Tim.seconds++;         //Increment seconds
        if (Tim.seconds >= 60) // If 60
        {
            Tim.seconds = 0;       // Reset to 0
            Tim.minutes++;         //Increment minutes
            if (Tim.minutes >= 60) // If 60
            {
                Tim.minutes = 0;     // Reset to 0
                Tim.hours++;         //Increment hours
                if (Tim.hours >= 24) // If 24
                { 
                    Tim.hours = 0; // Reset to 0
                }
            }
        }
        xQueueSend(xQueue1, &Tim, 0); //Send to Task 2
    }
}

//
// Read an integer number from the keyboard and retun to the calling program
//

unsigned int Read_From_Keyboard()
{
    unsigned int Total;
    unsigned char N;
    Total = 0;
    while (1)
    {
        if (UART_Data_Ready())
        {
            N = UART_receive(); //Read a number
            UART_send(N);       //Echo the number
            if (N == '\r')
                break;              //If Enter
            N = N - '0';            //Pure number
            Total = 10 * Total + N; //Total number
        }
    }
    return Total; //Return the number
}

//
// Read time from the keyboard. The time is entered as hh:mm:ss
//
void Read_Time(char buf[])
{
    unsigned char c, k = 0;
    while (1)
    {
        c = UART_receive(); // Read a char
        UART_send(c);       //  Echo the char
        if (c == '\r')
            break;  //If Enter
        buf[k] = c; //Save char
        k++;        //  Increment pointer
    }
    buf[k] = '\0'; //  NULL terminator
}

//
// 								Task 2 - LCD Controller
//
// A Task responsible for printing the time of the chosen city on the LCD
// with the correct time difference calculated to the chosen city
// defaults to 0: London
//

void Task2(void *pvParameters)
{
    char Txt[7];
    typedef struct Message
    {
        unsigned char hours;
        unsigned char minutes;
        unsigned char seconds;
    } AMessage;

    AMessage Tim;
    LCD_init();    //Initialize LCD
    LCD_clear();   //Clear LCD
    selection = 0; //Clear selection
    while (1)
    {
        xQueueReceive(xQueue1, &Tim, portMAX_DELAY); // Get time
        LCD_clear();
        LCD_print(cities[selection]);                //Display city
        Tim.hours = Tim.hours + timediff[selection]; // Hour adjustment
        LCD_secondLine();
        if (Tim.hours > 23)
            Tim.hours = Tim.hours - 24; // If > 24
        ByteToStr(Tim.hours, Txt);      // Convert to string

        LCD_print(Txt); // Display hours
        LCD_print(":"); //Colon

        ByteToStr(Tim.minutes, Txt); // To string
        LCD_print(Txt);              //Display seconds
        LCD_print(":");

        ByteToStr(Tim.seconds, Txt); // To string
        LCD_print(Txt);              //Display seconds
    }
}

//
// 											Task 3 - UART Controller
//
// A Task responsible for printing the starting message on the UART terminal
// and interacting with the user by showing the available cities and waiting
// for the user's input to choose a city from the list
//
void Task3(void *pvParameters)
{
    char k, Buffer[10];
    typedef struct Message
    {
        unsigned char hh;
        unsigned char mm;
        unsigned char ss;
    } AMessage;

    AMessage Tim;

    UART_init();
    UART_send_string("\n\rTime in Different Countries");
    UART_send_string("\n\r---------------------------");
    UART_send_string("\n\rEnter the time in London (hh:mm:ss): ");
    Read_Time(Buffer);
    Tim.hh = 10 * (Buffer[0] - '0') + Buffer[1] - '0'; // Convert to nmbr
    Tim.mm = 10 * (Buffer[3] - '0') + Buffer[4] - '0'; // Convert to nmbr
    Tim.ss = 10 * (Buffer[6] - '0') + Buffer[7] - '0'; // Convert to nmbr
    xQueueSend(xQueue2, &Tim, 0);                      // Send to Task 1
    while (1)
    {
        UART_send_string("\n\r\n\rSelect a City:"); //Heading
        for (k = 0; k < 10; k++)                    //Display cities
        {
            UART_send_string("\n\r"); //New line
            UART_send(k + '0');
            UART_send_string(". ");
            UART_send_string(cities[k]); //City names
        }
        UART_send_string("\n\rSelection: "); //Selection prompt
        selection = Read_From_Keyboard();    //Read selection
    }
}

//
// get a string of a number up to two digits
//
void ByteToStr(char ch, char b[])
{
    int i = 2;
    for (int j = 0; j < 2; j++)
    {
        b[j] = '0';
    }
    while (ch != 0)
    {
        i--;
        b[i] = ((ch % 10) + '0');
        ch /= 10;
    }
    b[2] = '\0';
}

//
//Start of MAIN program
//
int main()
{
    xQueue1 = xQueueCreate(1, 8); //Create queue
    xQueue2 = xQueueCreate(1, 8); //Create queue
    //
    //Create all the TASKS here
    //
    xTaskCreate(Taskl, "Time Controller", configMINIMAL_STACK_SIZE, NULL, 10, NULL);
    xTaskCreate(Task2, "LCD Controller", configMINIMAL_STACK_SIZE, NULL, 9, NULL);
    xTaskCreate(Task3, "UART Controller", configMINIMAL_STACK_SIZE, NULL, 9, NULL);
    vTaskStartScheduler(); //Start the RTOS scheduler
    return 0;
} //Will never reach here