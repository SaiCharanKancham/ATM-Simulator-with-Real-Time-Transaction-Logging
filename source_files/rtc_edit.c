//rtc_edit2.c
#include "rtc.h"
#include "lcd.h"
#include "kpm.h"
#include "delay.h"

s32 hour, min, sec, date, month, year;
u8 rchoice;

// Edit individual date fields
void Edit_Date(void)
{
    while(1)
    {
        CmdLCD(0x01);
        StrLCD("1:DATE 2:MONTH");
        CmdLCD(0xC0);
        StrLCD("3:YEAR 4:BACK");

        //rchoice = Input();//
		 while(ColScan());
         rchoice = KeyScan();
         while(ColScan()==0);

        switch(rchoice)
        {
            case '1':
                do
                {
                    CmdLCD(0x01);
                    StrLCD("ENTER DATE:");
                    date = Input();

                    if((date < 1) || (date > 31))
                    {
                        CmdLCD(0x01);
                        StrLCD("INVALID DATE");
                        delay_ms(1000);
                    }
                }while((date < 1) || (date > 31));

                SetRTCDateInfo(date, month, year);
                break;

            case '2':
                do
                {
                    CmdLCD(0x01);
                    StrLCD("ENTER MONTH:");
                    month = Input();

                    if((month < 1) || (month > 12))
                    {
                        CmdLCD(0x01);
                        StrLCD("INVALID MONTH");
                        delay_ms(1000);
                    }
                }while((month < 1) || (month > 12));

                SetRTCDateInfo(date, month, year);
                break;

            case '3':
                do
                {
                    CmdLCD(0x01);
                    StrLCD("ENTER YEAR:");
                    year = Input();

                    if((year < 2026) || (year > 2099))
                    {
                        CmdLCD(0x01);
                        StrLCD("INVALID YEAR");
                        delay_ms(1000);
                    }
                }while((year < 2026) || (year > 2099));

                SetRTCDateInfo(date, month, year);
                break;

            case '4':
                return;

            default:
                CmdLCD(0x01);
                StrLCD("INVALID OPTION");
                delay_ms(1000);
        }
    }
}

// Edit individual time fields
void Edit_Time(void)
{
    while(1)
    {
        CmdLCD(0x01);
        StrLCD("1:HOUR 2:MIN");
        CmdLCD(0xC0);
        StrLCD("3:SEC 4:BACK");

	   	 while(ColScan());
         rchoice = KeyScan();
         while(ColScan()==0);

        switch(rchoice)
        {
            case '1':
                do
                {
                    CmdLCD(0x01);
                    StrLCD("ENTER HOUR:");
                    hour = Input();

                    if((hour < 0) || (hour > 23))
                    {
                        CmdLCD(0x01);
                        StrLCD("INVALID HOUR");
                        delay_ms(1000);
                    }
                }while((hour < 0) || (hour > 23));

                SetRTCTimeInfo(hour, min, sec);
                break;

            case '2':
                do
                {
                    CmdLCD(0x01);
                    StrLCD("ENTER MIN:");
                    min = Input();

                    if((min < 0) || (min > 59))
                    {
                        CmdLCD(0x01);
                        StrLCD("INVALID MIN");
                        delay_ms(1000);
                    }
                }while((min < 0) || (min > 59));

                SetRTCTimeInfo(hour, min, sec);
                break;

            case '3':
                do
                {
                    CmdLCD(0x01);
                    StrLCD("ENTER SEC:");
                    sec = Input();

                    if((sec < 0) || (sec > 59))
                    {
                        CmdLCD(0x01);
                        StrLCD("INVALID SEC");
                        delay_ms(1000);
                    }
                }while((sec < 0) || (sec > 59));

                SetRTCTimeInfo(hour, min, sec);
                break;

            case '4':
                return;

            default:
                CmdLCD(0x01);
                StrLCD("INVALID OPTION");
                delay_ms(1000);
        }
    }
}
