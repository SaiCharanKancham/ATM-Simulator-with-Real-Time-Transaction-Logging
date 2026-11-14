
/*
 * File: uart.h
 * Description: Contains function declarations for UART communication.
 */

/* Prevent multiple inclusion of this header file. */
#ifndef UART_H
#define UART_H

/* Include user-defined data types. */
#include "types.h"

/* Initialize UART communication. */
void UART_Init(void);

/* Transmit a single byte through UART. */
void UART_Tx(u8 sdat);

/* Receive a single byte through UART. */
u8 UART_Rx(void);

/* Transmit a string through UART. */
void UART_Txstr(s8 *s);

/* Transmit a signed 32-bit integer through UART. */
void UART_TxS32(s32 n);

/* Transmit an unsigned 32-bit integer through UART. */
void UART_TxU32(u32 n);

/* Transmit a floating-point value with specified decimal places. */
void UART_TxF32(f32 n, u32 nDP);

/* End of header guard. */
#endif /* UART_H */
