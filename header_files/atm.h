
/*
 * File: atm.h
 * Description: Public declarations shared by main.c and atm_services.c.
 */

/* Prevent multiple inclusion of this header file. */
#ifndef ATM_H
#define ATM_H

/* Include user-defined data types. */
#include "types.h"

/*
 * Global flag used to restart account login.
 * Set by Pin_Verification() and checked by main().
 */
extern u8 pin_retry_restart;

/* Function declaration for displaying the ATM menu. */
void menu(void);

/* Function declaration for displaying available services. */
void Service_Menu(void);

/* Function declaration for PIN verification. */
void Pin_Verification(void);

/* ATM service function declarations. */
void Balance_Enquiry(void);
void Deposit(void);
void Withdraw(void);
void Change_Pin(void);
void Mini_Statement(void);

/* Function declaration for saving transaction details. */
void Save_Transaction(s8 *type, u32 amount);

/* End of header guard. */
#endif /* ATM_H */
