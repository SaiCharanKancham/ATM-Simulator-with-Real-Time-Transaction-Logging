//delay_def.c

// Delay in microseconds
void delay_us(unsigned int dlyUS)       // Function to generate delay in microseconds
{
    // for(dlyUS *= 12; dlyUS > 0; dlyUS--); // Alternative for-loop implementation
    dlyUS *= 12;                        // Multiplies the required delay by 12 to get the loop count
    while(dlyUS--);                     // Decrements the count until it reaches zero
}

// Delay in milliseconds
void delay_ms(unsigned int dlyMS)       // Function to generate delay in milliseconds
{
    // for(dlyMS *= 12000; dlyMS > 0; dlyMS--); // Alternative for-loop implementation
    dlyMS *= 12000;                     // Multiplies the required delay by 12000 to get the loop count
    while(dlyMS--);                     // Decrements the count until it reaches zero
}

// Delay in seconds
void delay_s(unsigned int dlyS)         // Function to generate delay in seconds
{
    // for(dlyS *= 12000000; dlyS > 0; dlyS--); // Alternative for-loop implementation
    dlyS *= 12000000;                   // Multiplies the required delay by 12000000 to get the loop count
    while(dlyS--);                      // Decrements the count until it reaches zero
}
