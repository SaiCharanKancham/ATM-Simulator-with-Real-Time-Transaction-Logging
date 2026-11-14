
/*
 * File: defines.h
 * Description: Contains macros for bit, nibble and byte manipulation.
 */

/* Prevent multiple inclusion of this header file. */
#ifndef DEFINES_H
#define DEFINES_H

/* Set the specified bit to 1. */
#define SETBIT(WORD,BITPOS) WORD|=1<<BITPOS

/* Clear the specified bit to 0. */
#define CLRBIT(WORD,BITPOS) WORD&=~(1<<BITPOS)

/* Toggle the specified bit (0 to 1 or 1 to 0). */
#define CPLBIT(WORD,BITPOS) WORD^=(1<<BITPOS)

/* Read the value of the specified bit (returns 0 or 1). */
#define READBIT(WORD,BITPOS) ((WORD>>BITPOS)&1)

/* Write the specified value (0 or 1) into a particular bit. */
#define WRITEBIT(WORD,BITPOS,BIT) WORD=(WORD &~(1<<BITPOS))|(BIT<<BITPOS)

/* Copy the value of one bit to another bit in the same register. */
#define READWRITEBIT(WORD,DBIT,SBIT) WORD=( (WORD&~(1<<DBIT)) |(((WORD>>SBIT)&1)<<DBIT) )

/* Copy a bit from one register to a bit in another register. */
#define READWRITEBIT2(DWORD,DBIT,SWORD,SBIT) DWORD=( (DWORD&~(1<<DBIT)) | (((SWORD>>SBIT)&1)<<DBIT) )

/* Write a 4-bit value (nibble) starting from the specified bit position. */
#define WRITENIBBLE(WORD,STARTBIT,VAL) WORD=( (WORD&~(15<<STARTBIT))|(VAL<<STARTBIT))

/* Read a 4-bit value (nibble) starting from the specified bit position. */
#define READNIBBLE(WORD,STARTBIT) (WORD>>STARTBIT)&15

/* Write an 8-bit value (byte) starting from the specified bit position. */
#define WRITEBYTE(WORD,STARTBIT,VAL) WORD=( (WORD&~(255<<STARTBIT))|(VAL<<STARTBIT))

/* Read an 8-bit value (byte) starting from the specified bit position. */
#define READBYTE(WORD,STARTBIT) ((WORD>>STARTBIT)&255)

/* End of header guard. */
#endif /* DEFINES_H */
