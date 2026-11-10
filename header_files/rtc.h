//rtc.h
#include "types.h"

#ifndef RTC_H
#define RTC_H

/* RTC Initialization */
void Init_RTC(void);
/* Set RTC Time */
void SetRTCTimeInfo(u32 hour, u32 minute, u32 second);
/* Get RTC Time */
void GetRTCTimeInfo(s32 *hour, s32 *minute, s32 *second);
/* Set RTC Date */
void SetRTCDateInfo(u32 date, u32 month, u32 year);
/* Get RTC Date */
void GetRTCDateInfo(s32 *date, s32 *month, s32 *year);

#endif
