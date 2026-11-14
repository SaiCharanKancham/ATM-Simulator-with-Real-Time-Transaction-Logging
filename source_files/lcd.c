
// lcd.c - Contains LCD interfacing and display functions

#include <LPC21xx.h>          // Includes LPC21xx microcontroller register definitions
#include "lcd_defines.h"      // Includes LCD pin connections and command definitions
#include "defines.h"          // Includes user-defined macros such as SETBIT and WRITEBYTE
#include "types.h"            // Includes user-defined data types
#include "delay.h"            // Includes delay function declarations

// Write a byte to LCD
void WriteLCD(u8 byte)        // Function to send a command or data byte to the LCD
{
    // Select write operation
    IOCLR0 = 1 << LCD_RW;     // Makes RW pin LOW to select write operation

    // Place any byte on data pins D0 to D7
    WRITEBYTE(IOPIN0, LCD_DATA, byte); // Places the given byte on the LCD data pins

    // Provide high-to-low pulse for latching
    IOSET0 = 1 << LCD_EN;     // Makes Enable pin HIGH to start the data transfer
    delay_us(1);              // Keeps Enable HIGH for 1 microsecond
    IOCLR0 = 1 << LCD_EN;     // Makes Enable pin LOW to latch the data into the LCD

    delay_ms(2);              // Waits for the LCD to complete the operation
}

// Send a command to LCD
void CmdLCD(u8 cmd)           // Function to send a command to the LCD
{
    // Select command register
    IOCLR0 = 1 << LCD_RS;     // Makes RS pin LOW to select the command register

    // Write the command to LCD
    WriteLCD(cmd);            // Sends the command byte to the LCD
}

// Initialize LCD
void InitLCD(void)            // Function to initialize the LCD
{
    // Configure LCD connection GPIO pins as outputs
    WRITEBYTE(IODIR0, LCD_DATA, 0xFF); // Configures LCD data pins D0-D7 as outputs
    SETBIT(IODIR0, LCD_RS);   // Configures RS pin as output
    SETBIT(IODIR0, LCD_RW);   // Configures RW pin as output
    SETBIT(IODIR0, LCD_EN);   // Configures Enable pin as output

    // Power-on delay
    delay_ms(15);             // Waits 15 milliseconds after powering on the LCD

    CmdLCD(0x30);              // Sends the initial LCD initialization command
    delay_ms(4);               // Waits 4 milliseconds for the LCD to process the command
    delay_us(1);               // Waits 1 microsecond

    CmdLCD(0x30);              // Sends the initialization command again
    delay_us(100);             // Waits 100 microseconds

    CmdLCD(0x30);              // Sends the initialization command for the third time

    CmdLCD(MODE_8BIT_2LINE);   // Selects 8-bit mode and two-line display
    CmdLCD(DSP_ON_CUR_OFF);    // Turns ON the display and turns OFF the cursor
    CmdLCD(CLEAR_LCD);         // Clears the LCD display
    CmdLCD(SHIFT_CUR_RIGHT);   // Configures the cursor to move right after each character
}

// Display a single character
void CharLCD(u8 asciiVal)     // Function to display one character on the LCD
{
    // Select data register
    IOSET0 = 1 << LCD_RS;     // Makes RS pin HIGH to select the data register

    // Write data to DDRAM through the data register
    WriteLCD(asciiVal);       // Sends the character to the LCD for display
}

// Display a string
void StrLCD(s8 *s)            // Function to display a string on the LCD
{
    while(*s)                 // Continues until the null character is reached
    {
        CharLCD(*s++);        // Displays the current character and moves to the next character
    }
}

// Display an unsigned number
void U32LCD(u32 n)            // Function to display an unsigned 32-bit integer
{
    s32 i = 0;                // Variable used as an index for the digit array
    u8 a[10];                 // Array to store the digits in reverse order

    if(n == 0)                // Checks whether the number is zero
    {
        CharLCD('0');         // Displays character '0'
    }
    else                      // Executes when the number is non-zero
    {
        while(n > 0)           // Extracts digits until the number becomes zero
        {
            a[i] = (n % 10) + 48; // Extracts the last digit and converts it to ASCII
            n = n / 10;        // Removes the last digit from the number
            i++;               // Moves to the next position in the digit array
        }

        for(--i; i >= 0; i--)  // Traverses the stored digits in reverse order
        {
            CharLCD(a[i]);     // Displays each digit in the correct order
        }
    }
}

// Display a signed number
void S32LCD(s32 n)            // Function to display a signed 32-bit integer
{
    if(n < 0)                 // Checks whether the number is negative
    {
        CharLCD('-');         // Displays the minus sign
        n = -n;               // Converts the number into its positive value
    }

    U32LCD(n);                // Displays the number using the unsigned number function
}

// Display a floating-point number
void F32LCD(f32 fn, u8 nDP)   // Function to display a float with the specified decimal places
{
    u32 n, i;                 // n stores the integer part and i is the loop counter

    if(fn < 0.0)              // Checks whether the floating-point number is negative
    {
        CharLCD('-');         // Displays the minus sign
        fn = -fn;             // Converts the number into its positive value
    }

    n = fn;                   // Extracts the integer part of the floating-point number
    U32LCD(n);                // Displays the integer part
    CharLCD('.');             // Displays the decimal point

    for(i = 0; i < nDP; i++)  // Repeats for the requested number of decimal places
    {
        fn = (fn - n) * 10;   // Extracts the next decimal digit
        n = fn;                // Stores the extracted digit as an integer
        CharLCD(n + 48);      // Converts the digit to ASCII and displays it
    }
}
