#include<LPC21xx.h>
#include"types.h"
//Initialisation
void UART_Init(void)
{
//cfg pinsel block
PINSEL0=(PINSEL0&~(3<<(0*2))) | (1<<(0*2));
PINSEL0=(PINSEL0&~(3<<(1*2))) | (1<<(1*2));
//uart block
U0LCR|=1<<7;
U0LCR|=3<<0;
U0DLL=0x61;
U0DLM=0;
U0LCR&=~(1<<7);
}
//Transmitting data
void UART_Tx(u8 sdat)
{
U0THR=sdat;
while( ((U0LSR>>6)&1)==0);
}
//Receiving data
u8 UART_Rx(void)
{
while( ((U0LSR>>0)&1)==0);
return  U0RBR;
}
void UART_Txstr(s8 *s)
{
while(*s)
{
UART_Tx(*s++);
}
}
void UART_TxU32(u32 n)
{
s32 i=0;
u8 a[10];
if(n==0)
{
UART_Tx('0');
}
else
{
while(n>0)
{
a[i++]=(n%10)+'0';
n=n/10;
}
for(--i;i>=0;i--)
{
UART_Tx(a[i]);
}
}
}
void UART_TxS32(s32 n)
{
if(n<0)
{
UART_Tx('-');
n=-n;
}
UART_TxU32(n);
}
void UART_TxF32(f32 n,u32 nDP)
{
u32 i,j=0;
if(n<0.0)
{
UART_Tx('-');
n=-n;
}
i=n;
UART_TxU32(i);
UART_Tx('.');
for(j=0;j<nDP;j++)
{
n=((n-i)*10);
i=n;
UART_Tx(i+'0');
}
}


