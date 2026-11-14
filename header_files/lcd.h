
/*
 * File: lcd.h
 * Description: Contains function declarations for LCD interfacing.
 */

/* Prevent multiple inclusion of this header file. */
#ifndef LCD_H
#define LCD_H

/* Include user-defined data types. */
#include "types.h"

/* Send a byte of data or command to the LCD. */
void WriteLCD(u8 byte);

/* Send a command instruction to the LCD. */
void CmdLCD(u8 cmd);

/* Initialize the LCD. */
void InitLCD(void);

/* Display a single ASCII character on the LCD. */
void CharLCD(u8 asciiVal);

/* Display a string on the LCD. */
void StrLCD(s8*);

/* Display an unsigned 32-bit integer on the LCD. */
void U32LCD(u32);

/* Display a signed 32-bit integer on the LCD. */
void S32LCD(s32);

/* Display a floating-point number with the specified decimal places. */
void F32LCD(f32, u8 nDP);

/* Create custom characters in the LCD CGRAM. */
void BuildCGRAM(u8 *p, u8 nBytes);

/* End of header guard. */
#endif /* LCD_H */
