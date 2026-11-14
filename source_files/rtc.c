
// rtc.c - Contains RTC initialization, time and date functions

#include <LPC214x.h>       // Includes LPC2148 register definitions
#include "rtc.h"          // Includes RTC function declarations and data types

#define FOSC 12000000     // Defines the external crystal frequency as 12 MHz
#define CCLK (5 * FOSC)   // Defines the CPU clock frequency as 5 times FOSC (60 MHz)
#define PCLK (CCLK / 4)   // Defines the peripheral clock frequency as CCLK divided by 4 (15 MHz)

#define PREINT_VAL  ((int)(PCLK / 32768) - 1)   // Calculates the integer part of the RTC prescaler
#define PREFRAC_VAL (PCLK - ((PREINT_VAL + 1) * 32768)) // Calculates the fractional part of the RTC prescaler

#define RTC_ENABLE (1 << 0)   // Bit 0 enables the RTC
#define RTC_RESET  (1 << 1)   // Bit 1 resets the RTC
#define RTC_CLKSRC (1 << 4)   // Bit 4 selects the RTC clock source

#define CPU_LPC2148            // Defines the target controller as LPC2148

/* RTC Initialization */
void Init_RTC(void)            // Initializes the RTC peripheral
{
    CCR = RTC_RESET;           // Resets the RTC counter and control registers

#ifndef CPU_LPC2148            // Compiles this section if the controller is not LPC2148

    PREINT = PREINT_VAL;       // Loads the integer prescaler value
    PREFRAC = PREFRAC_VAL;     // Loads the fractional prescaler value

    CCR = RTC_ENABLE;          // Enables the RTC (used for LPC2129)

#else                           // Compiles this section for LPC2148

    CCR = RTC_ENABLE | RTC_CLKSRC; // Enables the RTC and selects the external clock source

#endif                          // Ends the conditional compilation block
}

/* Set RTC Time */
void SetRTCTimeInfo(u32 hour, u32 minute, u32 second) // Sets the RTC time
{
    HOUR = hour;               // Loads the given hour value into the RTC
    MIN  = minute;             // Loads the given minute value into the RTC
    SEC  = second;             // Loads the given second value into the RTC
}

/* Get RTC Time */
void GetRTCTimeInfo(s32 *hour, s32 *minute, s32 *second) // Reads the RTC time using pointers
{
    *hour   = HOUR;            // Stores the RTC hour value at the address pointed to by hour
    *minute = MIN;             // Stores the RTC minute value at the address pointed to by minute
    *second = SEC;              // Stores the RTC second value at the address pointed to by second
}

/* Set RTC Date */
void SetRTCDateInfo(u32 date, u32 month, u32 year) // Sets the RTC date
{
    DOM   = date;              // Loads the given date into the Day of Month register
    MONTH = month;             // Loads the given month into the Month register
    YEAR  = year;              // Loads the given year into the Year register
}

/* Get RTC Date */
void GetRTCDateInfo(s32 *date, s32 *month, s32 *year) // Reads the RTC date using pointers
{
    *date  = DOM;              // Stores the RTC date at the address pointed to by date
    *month = MONTH;             // Stores the RTC month at the address pointed to by month
    *year  = YEAR;              // Stores the RTC year at the address pointed to by year
}
