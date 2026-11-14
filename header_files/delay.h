
/*
 * File: delay.h
 * Description: Contains function declarations for generating software delays.
 */

/* Prevent multiple inclusion of this header file. */
#ifndef DELAY_H
#define DELAY_H

/* Generate delay in microseconds. */
void delay_us(unsigned int dlyUS);

/* Generate delay in milliseconds. */
void delay_ms(unsigned int dlyMS);

/* Generate delay in seconds. */
void delay_s(unsigned int dlyS);

/* End of header guard. */
#endif /* DELAY_H */
