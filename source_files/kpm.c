//kpm_m.c
#include <LPC21xx.h>
#include "defines.h"
#include "types.h"
#include "kpm_defines.h"
#include "lcd_defines.h"
#include "lcd.h"

u32 kpmLUT[4][4]={{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};
u32 ColScan(void)
{
	u32 status;
	status=READNIBBLE(IOPIN1,COL0);
	return ((status<15) ? 0 : 1);
}

u32 RowCheck(void)
{
	u32 r;
	for(r=0;r<=3;r++)
	{
		WRITENIBBLE(IOPIN1,ROW0,~(1<<r));
		if(ColScan()==0)
		{
			break;
		}
	}
	WRITENIBBLE(IOPIN1,ROW0,0);
	return r;
}

u32 ColCheck(void)
{
	u32 c;
	for(c=0;c<=3;c++)
	{
		if(READBIT(IOPIN1,COL0+c)==0)
			break;
	}
	return c;
}

u32 KeyScan(void)
{
   u32 row,col,keyV;
   while(ColScan());
   row=RowCheck();
   col=ColCheck();
   keyV=kpmLUT[row][col];
   while((ColScan())==0);
   return keyV; 	
}
void InitKPM(void)
{
	//all rows grounded(output)
	IODIR1|=15<<ROW0;
	//all cols high (input)
	//IODIR1 &=~(15<<COL0); //default
}
u32 Input(void)
{
    u8 key;
    u32 sum=0;
    u32 digits=0;
    while(1)
    {
        key=KeyScan();
        if(key>='0' && key<='9')
        {
            sum=(sum*10)+(key-'0');
	        CmdLCD(GOTO_LINE2_POS0+(digits++));
            CharLCD(key);
        }
        else if(key=='*')
        {
            if(digits>0)
            {
                sum=sum/10;
                digits--;
                CmdLCD(0x10);
                CharLCD(' ');
                CmdLCD(0x10);
            }
        }
        else
        {
            break;
        }
    }
    return sum;
}
