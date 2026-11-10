/*
 * File: atm.h
 * Description: Public declarations shared by main.c and atm_services.c.
 */

#ifndef ATM_H
#define ATM_H

#include "types.h"

/* Set by Pin_Verification() and checked by main(). */
extern u8 pin_retry_restart;

/* ATM menu and service functions. */
void menu(void);
void Service_Menu(void);
void Pin_Verification(void);

void Balance_Enquiry(void);
void Deposit(void);
void Withdraw(void);
void Change_Pin(void);
void Mini_Statement(void);

void Save_Transaction(s8 *type, u32 amount);

#endif /* ATM_H */
