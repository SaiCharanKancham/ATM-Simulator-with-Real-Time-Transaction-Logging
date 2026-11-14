//uart.c
// Includes LPC21xx microcontroller register definitions
#include <LPC21xx.h>
// Includes user-defined data types such as u8, u32, s32 and f32
#include "types.h"

// UART Initialization
void UART_Init(void)                         // Function to initialize UART0
{
    // Configure PINSEL registers for UART0 pins
    PINSEL0 = (PINSEL0 & ~(3 << (0*2))) | (1 << (0*2)); // Configures P0.0 as TXD0
    PINSEL0 = (PINSEL0 & ~(3 << (1*2))) | (1 << (1*2)); // Configures P0.1 as RXD0
    // UART configuration
    U0LCR |= 1 << 7;                          // Sets DLAB bit to 1 to access baud rate registers
    U0LCR |= 3 << 0;                          // Configures UART for 8-bit data transmission (8 data bits)
    U0DLL = 0x61;                             // Loads the divisor latch low value for baud rate generation
    U0DLM = 0;                                // Loads the divisor latch high value as 0
    U0LCR &= ~(1 << 7);                       // Clears DLAB to 0 and enables access to UART data registers
}

// Transmitting data
void UART_Tx(u8 sdat)                         // Function to transmit one byte of data
{
    U0THR = sdat;                             // Loads the data into the UART transmit holding register
    while(((U0LSR >> 6) & 1) == 0);            // Waits until the transmitter is empty and the data is completely transmitted
}

// Receiving data
u8 UART_Rx(void)                              // Function to receive one byte of data
{
    while(((U0LSR >> 0) & 1) == 0);            // Waits until the receiver data ready bit becomes 1
    return U0RBR;                             // Reads and returns the received byte
}

// Transmitting a string
void UART_Txstr(s8 *s)                        // Function to transmit a string through UART
{
    while(*s)                                 // Continues until the null character is reached
    {
        UART_Tx(*s++);                        // Transmits the current character and moves to the next character
    }
}

// Transmitting an unsigned 32-bit integer
void UART_TxU32(u32 n)                        // Function to transmit an unsigned integer as ASCII digits
{
    s32 i = 0;                                // Variable used as an index for the digit array
    u8 a[10];                                 // Array to store the digits of the number in reverse order
    if(n == 0)                                // Checks whether the number is zero
    {
        UART_Tx('0');                         // Transmits character '0'
    }
    else                                      // Executes when the number is non-zero
    {
        while(n > 0)                           // Extracts digits until the number becomes zero
        {
            a[i++] = (n % 10) + '0';           // Extracts the last digit, converts it to ASCII and stores it
            n = n / 10;                       // Removes the last digit from the number
        }
        for(--i; i >= 0; i--)                 // Traverses the stored digits in reverse order
        {
            UART_Tx(a[i]);                    // Transmits each digit in the correct order
        }
    }
}

// Transmitting a signed 32-bit integer
void UART_TxS32(s32 n)                        // Function to transmit a signed integer
{
    if(n < 0)                                 // Checks whether the number is negative
    {
        UART_Tx('-');                         // Transmits the minus sign
        n = -n;                               // Converts the number into its positive value
    }
    UART_TxU32(n);                            // Transmits the number using the unsigned integer function
}

// Transmitting a floating-point number
void UART_TxF32(f32 n, u32 nDP)               // Function to transmit a float with the specified decimal places
{
    u32 i, j = 0;                             // i stores the integer part and j is the loop counter
    if(n < 0.0)                               // Checks whether the floating-point number is negative
    {
        UART_Tx('-');                         // Transmits the minus sign
        n = -n;                               // Converts the number into its positive value
    }
    i = n;                                    // Extracts the integer part of the floating-point number
    UART_TxU32(i);                            // Transmits the integer part
    UART_Tx('.');                             // Transmits the decimal point
    for(j = 0; j < nDP; j++)                  // Repeats for the requested number of decimal places
    {
        n = (n - i) * 10;                     // Extracts the next decimal digit
        i = n;                                // Stores the extracted digit as an integer
        UART_Tx(i + '0');                     // Converts the digit to ASCII and transmits it
    }
}
