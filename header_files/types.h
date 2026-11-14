
/*
 * File: types.h
 * Description: Defines user-defined data type aliases for embedded C programming.
 */

/* Prevent multiple inclusion of this header file. */
#ifndef TYPES_H
#define TYPES_H

/* 8-bit data types. */
typedef unsigned char u8;       // Unsigned 8-bit integer
typedef char s8;                // Signed 8-bit character

/* 16-bit data types. */
typedef unsigned short int u16; // Unsigned 16-bit integer
typedef short int s16;          // Signed 16-bit integer

/* 32-bit data types. */
typedef unsigned int u32;       // Unsigned 32-bit integer
typedef int s32;                // Signed 32-bit integer

/* Floating-point data types. */
typedef float f32;              // 32-bit floating-point type
typedef double f64;             // 64-bit floating-point type

/* End of header guard. */
#endif /* TYPES_H */
