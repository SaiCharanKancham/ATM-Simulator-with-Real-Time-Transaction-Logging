//uart.h

#include"types.h"

void UART_Init(void);
void UART_Tx(u8 sdat);
u8 UART_Rx(void);
void UART_Txstr(s8 *s);
void UART_TxS32(s32 n);
void UART_TxU32(u32 n);
void UART_TxF32(f32 n,u32 nDP);

