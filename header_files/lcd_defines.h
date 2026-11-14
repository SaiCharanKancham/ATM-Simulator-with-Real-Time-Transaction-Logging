
/*
 * File: lcd_defines.h
 * Description: Defines LCD commands, operating modes and GPIO pin assignments.
 */

/* Prevent multiple inclusion of this header file. */
#ifndef LCD_DEFINES_H
#define LCD_DEFINES_H

/* LCD control commands. */
#define CLEAR_LCD           0x01    // Clear display
#define RET_CUR_HOME        0x02    // Return cursor to home position

/* Cursor movement commands. */
#define SHIFT_CUR_RIGHT     0x06    // Move cursor to the right
#define SHIFT_CUR_LEFT      0x07    // Move cursor to the left

/* Display ON/OFF and cursor control commands. */
#define DSP_OFF             0x08    // Display OFF
#define DSP_ON_CUR_OFF      0x0C    // Display ON, cursor OFF
#define DSP_ON_CUR_ON       0x0E    // Display ON, cursor ON
#define DSP_ON_CUR_BLINK    0x0F    // Display ON, cursor blinking

/* Display shifting commands. */
#define SHIFT_DISP_LEFT     0x10    // Shift entire display to the left
#define SHIFT_DISP_RIGHT    0x14    // Shift entire display to the right

/* LCD interface mode configuration. */
#define MODE_8BIT_1LINE     0x30    // 8-bit mode, 1-line display
#define MODE_4BIT_1LINE     0x20    // 4-bit mode, 1-line display
#define MODE_8BIT_2LINE     0x38    // 8-bit mode, 2-line display
#define MODE_4BIT_2LINE     0x28    // 4-bit mode, 2-line display

/* LCD cursor positioning commands. */
#define GOTO_LINE1_POS0     0x80    // Move cursor to line 1, position 0
#define GOTO_LINE2_POS0     0xC0    // Move cursor to line 2, position 0
#define GOTO_LINE3_POS0     0x94    // Move cursor to line 3, position 0
#define GOTO_LINE4_POS0     0xD4    // Move cursor to line 4, position 0

/* CGRAM address configuration. */
#define GOTO_CGRAM_START    0x40    // Set CGRAM starting address

/* LCD GPIO pin definitions (LPC2148 Port 0). */
#define LCD_DATA            8       // LCD data pins start from P0.8
#define LCD_RS              16      // Register Select pin - P0.16
#define LCD_RW              17      // Read/Write pin - P0.17
#define LCD_EN              18      // Enable pin - P0.18

/* End of header guard. */
#endif /* LCD_DEFINES_H */
