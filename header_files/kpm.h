
/*
 * File: kpm_m.h
 * Description: Contains function declarations for 4x4 matrix keypad operations.
 */

/* Prevent multiple inclusion of this header file. */
#ifndef KPM_H
#define KPM_H

/* Include user-defined data types. */
#include "types.h"

/* Scan keypad columns to identify an active column. */
u32 ColScan(void);

/* Check keypad rows to identify a pressed key. */
u32 RowCheck(void);

/* Check keypad columns to identify a pressed key. */
u32 ColCheck(void);

/* Scan the keypad and identify the pressed key. */
u32 KeyScan(void);

/* Initialize the GPIO pins used for keypad interfacing. */
void InitKPM(void);

/* Read and return the keypad input. */
u32 Input(void);

/* End of header guard. */
#endif /* KPM_H */
