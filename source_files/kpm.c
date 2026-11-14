
// kpm_m.c - Contains keypad interfacing and input handling functions

#include <LPC21xx.h>       // Includes LPC21xx microcontroller register definitions
#include "defines.h"       // Includes user-defined macros such as READBIT and WRITENIBBLE
#include "types.h"         // Includes user-defined data types
#include "kpm_defines.h"   // Includes keypad pin connection definitions
#include "lcd_defines.h"   // Includes LCD command and position definitions
#include "lcd.h"           // Includes LCD function declarations

// Keypad lookup table
u8 kpmLUT[4][4] = {         // Stores the character corresponding to each keypad row and column
    {'1','2','3','A'},     // First row of keypad
    {'4','5','6','B'},     // Second row of keypad
    {'7','8','9','C'},     // Third row of keypad
    {'*','0','#','D'}      // Fourth row of keypad
};

// Check whether any key is pressed
u32 ColScan(void)           // Function to scan all keypad columns
{
    u32 status;             // Variable to store the column input status
    status = READNIBBLE(IOPIN1, COL0); // Reads the four column input pins
    return ((status < 15) ? 0 : 1);    // Returns 0 if any column is LOW, otherwise returns 1
}

// Identify the pressed keypad row
u32 RowCheck(void)          // Function to identify which row contains the pressed key
{
    u32 r;                  // Loop variable used to select each row
    for(r = 0; r <= 3; r++) // Checks all four keypad rows one by one
    {
        WRITENIBBLE(IOPIN1, ROW0, ~(1 << r)); // Makes the selected row LOW and other rows HIGH
        if(ColScan() == 0)  // Checks whether a key press is detected in the selected row
        {
            break;          // Exits the loop when the pressed row is identified
        }
    }

    WRITENIBBLE(IOPIN1, ROW0, 0); // Clears the row output bits after scanning
    return r;               // Returns the detected row number
}

// Identify the pressed keypad column
u32 ColCheck(void)          // Function to identify which column contains the pressed key
{
    u32 c;                  // Loop variable used to check each column

    for(c = 0; c <= 3; c++) // Checks all four keypad columns one by one
    {
        if(READBIT(IOPIN1, COL0 + c) == 0) // Checks whether the current column is LOW
            break;          // Exits the loop when the pressed column is identified
    }
    return c;               // Returns the detected column number
}

// Scan and identify the pressed key
u32 KeyScan(void)           // Function to return the character of the pressed keypad key
{
    u32 row, col, keyV;     // Variables to store the row, column and key value

    while(ColScan());       // Waits until a key is pressed
    row = RowCheck();       // Identifies the pressed keypad row
    col = ColCheck();       // Identifies the pressed keypad column
    keyV = kpmLUT[row][col]; // Gets the key character from the lookup table
    while((ColScan()) == 0); // Waits until the pressed key is released
    return keyV;            // Returns the detected keypad character
}

// Initialize keypad
void InitKPM(void)          // Function to initialize keypad GPIO pins
{
    // Configure all rows as outputs
    IODIR1 |= 15 << ROW0;   // Sets all four row pins as GPIO outputs
    // Configure all columns as inputs
    // IODIR1 &= ~(15 << COL0); // Clears column direction bits; columns remain inputs by default
}

// Read a numeric value from keypad
u32 Input(void)             // Function to read multiple digits and return the entered number
{
    u8 key;                 // Variable to store the pressed keypad key
    u32 sum = 0;            // Stores the numeric value entered by the user
    u32 digits = 0;         // Counts the number of digits entered

    while(1)                // Continuously reads keypad input until a terminating key is pressed
    {
        key = KeyScan();    // Reads the pressed key from the keypad

        if(key >= '0' && key <= '9') // Checks whether the pressed key is a numeric digit
        {
            sum = (sum * 10) + (key - '0'); // Converts the ASCII digit into a number and adds it to the result
            CmdLCD(GOTO_LINE2_POS0 + (digits++)); // Moves the LCD cursor to the next position on the second line
            CharLCD(key);   // Displays the entered digit on the LCD
        }
        else if(key == '*') // Checks whether the backspace key '*' is pressed
        {
            if(digits > 0)  // Checks whether at least one digit has been entered
            {
                sum = sum / 10; // Removes the last entered digit from the numeric value
                digits--;   // Decreases the digit count by one
                CmdLCD(0x10); // Moves the LCD cursor one position to the left
                CharLCD(' '); // Replaces the previous digit with a blank space
                CmdLCD(0x10); // Moves the LCD cursor one more position to the left
            }
        }
        else                // Executes when a non-numeric key other than '*' is pressed
        {
            break;          // Stops accepting input and exits the loop
        }
    }

    return sum;             // Returns the final numeric value entered by the user
}
