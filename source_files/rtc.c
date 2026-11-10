//rtc.c
#include <LPC214x.h>
#include "rtc.h"

#define FOSC 12000000
#define CCLK (5 * FOSC)
#define PCLK (CCLK / 4)

#define PREINT_VAL  ((int)(PCLK / 32768) - 1)
#define PREFRAC_VAL (PCLK - ((PREINT_VAL + 1) * 32768))

#define RTC_ENABLE (1 << 0)
#define RTC_RESET  (1 << 1)
#define RTC_CLKSRC (1<<4) 

#define CPU_LPC2148
/* RTC Initialization */
void Init_RTC(void)
{
  // Disable and reset the RTC
	CCR = RTC_RESET;
  #ifndef CPU_LPC2148
  // Set prescaler integer and fractional parts
	PREINT = PREINT_VAL;
	PREFRAC = PREFRAC_VAL;
   // Enable the RTC
	CCR = RTC_ENABLE;  //LPC_2129
	#else
	// Enable the RTC with external clock source
	CCR = RTC_ENABLE | RTC_CLKSRC;  //LPC_2148
	#endif
}
/* Set RTC Time */
void SetRTCTimeInfo(u32 hour, u32 minute, u32 second)
{
    HOUR = hour;
    MIN  = minute;
    SEC  = second;
}

/* Get RTC Time */
void GetRTCTimeInfo(s32 *hour, s32 *minute, s32 *second)
{
    *hour   = HOUR;
    *minute = MIN;
    *second = SEC;
}
/* Set RTC Date */
void SetRTCDateInfo(u32 date, u32 month, u32 year)
{
    DOM   = date;
    MONTH = month;
    YEAR  = year;
}
/* Get RTC Date */
void GetRTCDateInfo(s32 *date, s32 *month, s32 *year)
{
    *date  = DOM;
    *month = MONTH;
    *year  = YEAR;
}
