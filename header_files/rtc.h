
/*
 * File: rtc.h
 * Description: Contains function declarations for RTC initialization,
 *              time configuration and date configuration.
 */

/* Prevent multiple inclusion of this header file. */
#ifndef RTC_H
#define RTC_H

/* Include user-defined data types. */
#include "types.h"

/* Initialize the Real-Time Clock (RTC) peripheral. */
void Init_RTC(void);

/* Set RTC time (hour, minute and second). */
void SetRTCTimeInfo(u32 hour, u32 minute, u32 second);

/* Get RTC time through pointer arguments. */
void GetRTCTimeInfo(s32 *hour, s32 *minute, s32 *second);

/* Set RTC date (date, month and year). */
void SetRTCDateInfo(u32 date, u32 month, u32 year);

/* Get RTC date through pointer arguments. */
void GetRTCDateInfo(s32 *date, s32 *month, s32 *year);

/* End of header guard. */
#endif /* RTC_H */
