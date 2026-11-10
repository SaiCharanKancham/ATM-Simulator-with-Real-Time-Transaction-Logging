//lcd.c
#include <LPC21xx.h>
#include "lcd_defines.h"
#include "defines.h"
#include  "types.h"
#include  "delay.h"

void WriteLCD(u8 byte)
{
//select write operation
IOCLR0=1<<LCD_RW;
//place any byte on data pins d0 to d7
WRITEBYTE(IOPIN0,LCD_DATA,byte);
//provide high to low pluse for latching
IOSET0=1<<LCD_EN;
delay_us(1);
IOCLR0=1<<LCD_EN;
delay_ms(2);
}

void CmdLCD(u8 cmd)
{
//select cmd register
IOCLR0=1<<LCD_RS;
//write any cmd to lcd
WriteLCD(cmd);
}

void InitLCD(void)
{
//cfg lcd connection gpio output pins
WRITEBYTE(IODIR0,LCD_DATA,0xff);
SETBIT(IODIR0,LCD_RS);
SETBIT(IODIR0,LCD_RW);
SETBIT(IODIR0,LCD_EN);

//power on delay
delay_ms(15);
CmdLCD(0x30);
delay_ms(4);
delay_us(1);
CmdLCD(0x30);
delay_us(100);
CmdLCD(0x30);
CmdLCD(MODE_8BIT_2LINE);
CmdLCD(DSP_ON_CUR_OFF);
CmdLCD(CLEAR_LCD);
CmdLCD(SHIFT_CUR_RIGHT);
}

//for character
void CharLCD(u8 asciiVal)
{
//select data register
IOSET0=1<<LCD_RS;
//write to ddram via data register
WriteLCD(asciiVal);
}

//for string
void StrLCD(s8 *s)
{
while(*s)
{
CharLCD(*s++);
}
}

//for unsigned number
void U32LCD(u32 n)
{
s32 i=0;
u8 a[10];
if(n==0)
{
CharLCD('0');
}
else
{
while(n>0)
{
a[i]=(n%10)+48;
n=n/10;
i++;
}
for(--i;i>=0;i--)
{
CharLCD(a[i]);
}
}
}

//for signed number
void S32LCD(s32 n)
{
if(n<0)
{
CharLCD('-');
n=-n;
}
U32LCD(n);
}

//for float
void F32LCD(f32 fn,u8 nDP)
{
u32 n,i;
if(fn<0.0)
{
CharLCD('-');
fn=-fn;
}
n=fn;
U32LCD(n);
CharLCD('.');
for(i=0;i<nDP;i++)
{
  fn=(fn-n)*10;
  n=fn;
  CharLCD(n+48);
}
}
