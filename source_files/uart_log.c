//Uart_log.c
#include<LPC21xx.h>
#include<stdio.h>
#include "types.h"
#include "uart.h"

//UART LOGGING FUNCTIONS

/* Send a string through UART */
void Log_Message(s8*msg)
{
    UART_Txstr(msg);
}

/* Send a number through UART */
void Log_Number(u32 num)
{
    UART_TxU32(num);
}

/* Send current RTC date and time */
void Log_Date_Time(void)
{
    s8 data[40];
    sprintf((char *)data,"[%02d/%02d/%04d %02d:%02d:%02d] ",
                   DOM, MONTH, YEAR, HOUR, MIN, SEC);
    Log_Message(data);
}

/* Send timestamp and event description */
void Log_Event(s8 *msg)
{
    Log_Date_Time();
    Log_Message(msg);
    Log_Message("\r\n");
}

/* Log account-related information */
void Log_data(void)
{
    Log_Event("Account: 11111");
}
