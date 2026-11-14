
/*
 * File: uart_log.h
 * Description: Contains function declarations for UART-based event
 *              logging and transaction data logging.
 */

/* Prevent multiple inclusion of this header file. */
#ifndef UART_LOG_H
#define UART_LOG_H

/* Include user-defined data types. */
#include "types.h"

/* Transmit a message through UART logging. */
void Log_Message(s8 *msg);

/* Log a numeric value through UART. */
void Log_Number(u32 num);

/* Log the current RTC date and time. */
void Log_Date_Time(void);

/* Log an event message through UART. */
void Log_Event(u8 *msg);

/* Log ATM transaction details. */
void Log_data(void);

/* End of header guard. */
#endif /* UART_LOG_H */
