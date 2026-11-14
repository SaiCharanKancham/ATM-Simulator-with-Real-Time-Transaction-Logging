
// Uart_log.c - Contains UART logging functions

#include <LPC21xx.h>       // Includes LPC21xx microcontroller register definitions
#include <stdio.h>          // Includes sprintf() function declaration
#include "types.h"          // Includes user-defined data types
#include "uart.h"           // Includes UART function declarations

// UART LOGGING FUNCTIONS

/* Send a string through UART */
void Log_Message(s8 *msg)  // Function to send a string through UART
{
    UART_Txstr(msg);        // Calls UART_Txstr() to transmit the complete string
}

/* Send a number through UART */
void Log_Number(u32 num)   // Function to send an unsigned integer through UART
{
    UART_TxU32(num);        // Calls UART_TxU32() to convert and transmit the number
}

/* Send current RTC date and time */
void Log_Date_Time(void)   // Function to send the current RTC date and time
{
    s8 data[40];            // Declares a character array to store the formatted date and time

    sprintf((char *)data,   // Converts the date and time values into a formatted string
            "[%02d/%02d/%04d %02d:%02d:%02d] ",
            DOM, MONTH, YEAR, HOUR, MIN, SEC); // Inserts RTC date, month, year, hour, minute and second

    Log_Message(data);      // Sends the formatted date and time through UART
}

/* Send timestamp and event description */
void Log_Event(s8 *msg)    // Function to log an event along with its timestamp
{
    Log_Date_Time();        // Sends the current RTC date and time
    Log_Message(msg);       // Sends the event description through UART
    Log_Message("\r\n");    // Sends carriage return and newline to move to the next line
}

/* Log account-related information */
void Log_data(void)        // Function to log account-related information
{
    Log_Event("Account: 11111"); // Logs the account number with the current RTC timestamp
}
