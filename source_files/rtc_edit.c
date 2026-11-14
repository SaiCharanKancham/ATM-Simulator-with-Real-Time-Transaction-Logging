
// rtc_edit.c - Contains functions to edit RTC date and time fields

#include "rtc.h"       // Includes RTC function declarations
#include "lcd.h"       // Includes LCD function declarations
#include "kpm.h"       // Includes keypad function declarations
#include "delay.h"     // Includes delay function declarations

s32 hour, min, sec, date, month, year; // Global variables to store RTC time and date values
u8 rchoice;                            // Stores the keypad choice entered by the user

// Edit individual date fields
void Edit_Date(void)                   // Function to edit date, month and year separately
{
    while(1)                           // Repeats the date editing menu until the user selects Back
    {
        CmdLCD(0x01);                  // Clears the LCD display
        StrLCD("1:DATE 2:MONTH");      // Displays date and month options on the first LCD line
        CmdLCD(0xC0);                  // Moves the LCD cursor to the second line
        StrLCD("3:YEAR 4:BACK");       // Displays year and back options

        // Wait until a key is pressed and read the key value
        while(ColScan());              // Waits while a key press is detected
        rchoice = KeyScan();           // Reads the pressed key from the keypad
        while(ColScan()==0);           // Waits until the key is released

        switch(rchoice)                // Checks which option was selected
        {
            case '1':                  // Executes when the user selects Date
                do                     // Repeats until a valid date is entered
                {
                    CmdLCD(0x01);      // Clears the LCD display
                    StrLCD("ENTER DATE:"); // Asks the user to enter the date
                    date = Input();    // Reads the date entered through the keypad

                    if((date < 1) || (date > 31)) // Checks whether the date is outside the allowed range
                    {
                        CmdLCD(0x01);  // Clears the LCD display
                        StrLCD("INVALID DATE"); // Displays an invalid date message
                        delay_ms(1000); // Waits for one second
                    }
                }while((date < 1) || (date > 31)); // Repeats if the date is less than 1 or greater than 31

                SetRTCDateInfo(date, month, year); // Updates the RTC with the new date and existing month and year
                break;                 // Exits the switch statement

            case '2':                  // Executes when the user selects Month
                do                     // Repeats until a valid month is entered
                {
                    CmdLCD(0x01);      // Clears the LCD display
                    StrLCD("ENTER MONTH:"); // Asks the user to enter the month
                    month = Input();   // Reads the month entered through the keypad

                    if((month < 1) || (month > 12)) // Checks whether the month is outside the allowed range
                    {
                        CmdLCD(0x01);  // Clears the LCD display
                        StrLCD("INVALID MONTH"); // Displays an invalid month message
                        delay_ms(1000); // Waits for one second
                    }
                }while((month < 1) || (month > 12)); // Repeats if the month is less than 1 or greater than 12

                SetRTCDateInfo(date, month, year); // Updates the RTC with the existing date and year and new month
                break;                 // Exits the switch statement

            case '3':                  // Executes when the user selects Year
                do                     // Repeats until a valid year is entered
                {
                    CmdLCD(0x01);      // Clears the LCD display
                    StrLCD("ENTER YEAR:"); // Asks the user to enter the year
                    year = Input();    // Reads the year entered through the keypad

                    if((year < 2026) || (year > 2099)) // Checks whether the year is outside the allowed range
                    {
                        CmdLCD(0x01);  // Clears the LCD display
                        StrLCD("INVALID YEAR"); // Displays an invalid year message
                        delay_ms(1000); // Waits for one second
                    }
                }while((year < 2026) || (year > 2099)); // Repeats if the year is less than 2026 or greater than 2099

                SetRTCDateInfo(date, month, year); // Updates the RTC with the new year and existing date and month
                break;                 // Exits the switch statement

            case '4':                  // Executes when the user selects Back
                return;                // Exits the Edit_Date() function

            default:                   // Executes when an invalid key is pressed
                CmdLCD(0x01);          // Clears the LCD display
                StrLCD("INVALID OPTION"); // Displays an invalid option message
                delay_ms(1000);        // Waits for one second
        }
    }
}

// Edit individual time fields
void Edit_Time(void)                   // Function to edit hour, minute and second separately
{
    while(1)                           // Repeats the time editing menu until the user selects Back
    {
        CmdLCD(0x01);                  // Clears the LCD display
        StrLCD("1:HOUR 2:MIN");        // Displays hour and minute options on the first LCD line
        CmdLCD(0xC0);                  // Moves the LCD cursor to the second line
        StrLCD("3:SEC 4:BACK");        // Displays second and back options

        // Wait until a key is pressed and read the key value
        while(ColScan());              // Waits while a key press is detected
        rchoice = KeyScan();           // Reads the pressed key from the keypad
        while(ColScan()==0);           // Waits until the key is released

        switch(rchoice)                // Checks which option was selected
        {
            case '1':                  // Executes when the user selects Hour
                do                     // Repeats until a valid hour is entered
                {
                    CmdLCD(0x01);      // Clears the LCD display
                    StrLCD("ENTER HOUR:"); // Asks the user to enter the hour
                    hour = Input();    // Reads the hour entered through the keypad

                    if((hour < 0) || (hour > 23)) // Checks whether the hour is outside the allowed range
                    {
                        CmdLCD(0x01);  // Clears the LCD display
                        StrLCD("INVALID HOUR"); // Displays an invalid hour message
                        delay_ms(1000); // Waits for one second
                    }
                }while((hour < 0) || (hour > 23)); // Repeats if the hour is less than 0 or greater than 23

                SetRTCTimeInfo(hour, min, sec); // Updates the RTC with the new hour and existing minute and second
                break;                 // Exits the switch statement

            case '2':                  // Executes when the user selects Minute
                do                     // Repeats until a valid minute is entered
                {
                    CmdLCD(0x01);      // Clears the LCD display
                    StrLCD("ENTER MIN:"); // Asks the user to enter the minute
                    min = Input();     // Reads the minute entered through the keypad

                    if((min < 0) || (min > 59)) // Checks whether the minute is outside the allowed range
                    {
                        CmdLCD(0x01);  // Clears the LCD display
                        StrLCD("INVALID MIN"); // Displays an invalid minute message
                        delay_ms(1000); // Waits for one second
                    }
                }while((min < 0) || (min > 59)); // Repeats if the minute is less than 0 or greater than 59

                SetRTCTimeInfo(hour, min, sec); // Updates the RTC with the existing hour and second and new minute
                break;                 // Exits the switch statement

            case '3':                  // Executes when the user selects Second
                do                     // Repeats until a valid second is entered
                {
                    CmdLCD(0x01);      // Clears the LCD display
                    StrLCD("ENTER SEC:"); // Asks the user to enter the second
                    sec = Input();     // Reads the second entered through the keypad

                    if((sec < 0) || (sec > 59)) // Checks whether the second is outside the allowed range
                    {
                        CmdLCD(0x01);  // Clears the LCD display
                        StrLCD("INVALID SEC"); // Displays an invalid second message
                        delay_ms(1000); // Waits for one second
                    }
                }while((sec < 0) || (sec > 59)); // Repeats if the second is less than 0 or greater than 59

                SetRTCTimeInfo(hour, min, sec); // Updates the RTC with the new second and existing hour and minute
                break;                 // Exits the switch statement

            case '4':                  // Executes when the user selects Back
                return;                // Exits the Edit_Time() function

            default:                   // Executes when an invalid key is pressed
                CmdLCD(0x01);          // Clears the LCD display
                StrLCD("INVALID OPTION"); // Displays an invalid option message
                delay_ms(1000);        // Waits for one second
        }
    }
}
