/*
 * File: atm_services.c
 * Description:
 *     Contains the ATM application services and menu handling.
 *
 *     Main features:
 *       - Balance enquiry, deposit and withdrawal
 *       - PIN change and PIN verification
 *       - Storage and display of the latest two transactions
 *       - LCD-based main and service menus
 *       - UART event logging
 *       - Restart request after three incorrect PIN attempts
 *
 * Note:
 *     This source file is compiled separately. Header guards belong in
 *     header (.h) files, not in this .c file.
 */

#include <LPC214x.h>
#include <string.h>
#include <stdio.h>
#include "lcd.h"
#include "delay.h"
#include "lcd_defines.h"
#include "kpm.h"
#include "types.h"
#include "atm.h"
#include "uart_log.h"
#include "uart.h"

/* Account credentials and transaction state maintained in RAM. */
u8 pin[5] = "2222";
u32 bal = 1000;
u8 stat_chng_pin = 0;
u8 choice;

/* Set when PIN attempts are exhausted; main() checks this flag. */
u8 pin_retry_restart = 0;

/* trans1 = newest transaction; trans2 = previous transaction. */
char trans1[20] = "";
char trans2[20] = "";
u8 trans_count = 0;

/* =====================================================
   TRANSACTION STORAGE
   ===================================================== */

/*
 * Save_Transaction()
 * Keeps only the two most recent transactions.
 * The previous latest entry is shifted to trans2 before trans1 is updated.
 */
void Save_Transaction(s8 *type, u32 amount)
{
    /* Shift the latest transaction into the previous-transaction slot. */
    strcpy(trans2, trans1);

    sprintf(trans1, "%s:%lu",
            (char *)type, (unsigned long)amount);

    /* There can be at most two valid transaction records. */
    if(trans_count < 2)
        trans_count++;

    Log_Event("Transaction saved");

    Log_Message("Transaction type: ");
    Log_Message((char *)type);
    Log_Message("\r\n");

    Log_Message("Transaction amount: ");
    Log_Number(amount);
    Log_Message("\r\n");
}

/* =====================================================
   BALANCE ENQUIRY
   ===================================================== */

/*
 * Balance_Enquiry()
 * Displays the current account balance on the LCD and logs it over UART.
 */
void Balance_Enquiry(void)
{
    Log_Event("Balance enquiry started");

    CmdLCD(0x01);
    CmdLCD(GOTO_LINE1_POS0);
    StrLCD("Avail Balance:");

    CmdLCD(GOTO_LINE2_POS0);
    U32LCD(bal);

    Log_Message("Available balance: ");
    Log_Number(bal);
    Log_Message("\r\n");

    delay_ms(2000);

    Log_Event("Balance enquiry completed");
}

/* =====================================================
   DEPOSIT
   ===================================================== */

/*
 * Deposit()
 * Reads an amount, validates it, updates the balance and records
 * the transaction only when the deposit is accepted.
 */
void Deposit(void)
{
    u32 amnt_dpt;

    Log_Event("Deposit transaction started");

    CmdLCD(0x01);
    CmdLCD(GOTO_LINE1_POS0);
    StrLCD("Enter Amnt:");

    Log_Event("Waiting for deposit amount");

    amnt_dpt = Input();

    Log_Message("Deposit amount entered: ");
    Log_Number(amnt_dpt);
    Log_Message("\r\n");

    /* Accept only positive amounts in multiples of 100 or 500. */
    if(amnt_dpt <= 0 ||
       (((amnt_dpt % 100) != 0) &&
        ((amnt_dpt % 500) != 0)))
    {
        Log_Event("Deposit rejected: invalid amount");

        CmdLCD(0x01);
        StrLCD("Invalid Amnt");

        delay_ms(1000);
    }
    else
    {
        bal = bal + amnt_dpt;

        CmdLCD(0x01);
        StrLCD("Success");

        Log_Event("Deposit successful");

        Log_Message("Updated balance: ");
        Log_Number(bal);
        Log_Message("\r\n");

        Save_Transaction("Deposit", amnt_dpt);

        delay_ms(1000);
    }

    Log_Event("Deposit function completed");
}

/* =====================================================
   WITHDRAWAL
   ===================================================== */

/*
 * Withdraw()
 * Checks the requested amount against the available balance and
 * denomination rules before deducting money and saving the transaction.
 */
void Withdraw(void)
{
    u32 amnt_wth_drw;

    Log_Event("Withdrawal transaction started");

    CmdLCD(0x01);
    CmdLCD(GOTO_LINE1_POS0);
    StrLCD("Enter Amnt:");

    Log_Event("Waiting for withdrawal amount");

    amnt_wth_drw = Input();

    Log_Message("Withdrawal amount entered: ");
    Log_Number(amnt_wth_drw);
    Log_Message("\r\n");

    /* Reject zero, invalid denominations and amounts above the balance. */
    if((amnt_wth_drw > bal) ||
       (amnt_wth_drw == 0) ||
       (((amnt_wth_drw % 100) != 0) &&
        ((amnt_wth_drw % 500) != 0)))
    {
        Log_Event("Withdrawal rejected");

        if(amnt_wth_drw > bal)
        {
            Log_Event("Reason: insufficient balance");

            CmdLCD(0x01);
            StrLCD("Insuff Bal");

            delay_ms(1000);
        }
        else if(amnt_wth_drw == 0)
        {
            Log_Event("Reason: zero amount");

            CmdLCD(0x01);
            StrLCD("Invalid Amnt");

            delay_ms(1000);
        }
        else
        {
            Log_Event("Reason: invalid input");

            CmdLCD(0x01);
            StrLCD("Invalid Amnt");

            delay_ms(1000);
        }
    }
    else
    {
        bal = bal - amnt_wth_drw;

        CmdLCD(0x01);
        StrLCD("Success");

        Log_Event("Withdrawal successful");

        Log_Message("Updated balance: ");
        Log_Number(bal);
        Log_Message("\r\n");

        Save_Transaction("Withdraw", amnt_wth_drw);

        delay_ms(1000);
    }

    Log_Event("Withdrawal function completed");
}

/* =====================================================
   CHANGE PIN
   ===================================================== */

/*
 * Change_Pin()
 * Reads a new four-digit PIN. The LCD displays '*' characters so the
 * entered PIN is not shown on the screen.
 */
void Change_Pin(void)
{
    u8 i = 0;
    u8 new_pin[5] = "    ";

    Log_Event("PIN change started");

    CmdLCD(0x01);
    CmdLCD(GOTO_LINE1_POS0);
    StrLCD("New 4-dig Pin:");

    Log_Event("Waiting for new PIN input");

    while(i < 4)
    {
        while(ColScan());

        new_pin[i] = KeyScan();

        while(ColScan() == 0);

        CmdLCD(GOTO_LINE2_POS0 + i);
        CharLCD('*');

        i++;
    }

    for(i = 0; i < 4; i++)
    {
        if(new_pin[i] < '0' || new_pin[i] > '9')
        {
            Log_Event("PIN change failed: invalid digit");

            CmdLCD(0x01);
            StrLCD("Invalid PIN");

            delay_ms(1000);
            return;
        }
    }

    /* Store the new PIN only after all four characters pass validation. */
    strcpy((char *)pin, (char *)new_pin);

    CmdLCD(0x01);
    StrLCD("PIN Updated");

    Log_Event("PIN changed successfully");

    delay_ms(1000);

    stat_chng_pin = 0;

    Log_Event("PIN change completed");
}

/* =====================================================
   MINI STATEMENT
   ===================================================== */

/*
 * Mini_Statement()
 * Displays zero, one or two saved transactions depending on trans_count.
 */
void Mini_Statement(void)
{
    Log_Event("Mini statement requested");

    CmdLCD(0x01);
    CmdLCD(GOTO_LINE1_POS0);
    StrLCD("Mini Statement");

    if(trans_count == 0)
    {
        Log_Event("No transactions available");

        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("No Transactions");

        delay_ms(1500);
    }
    else if(trans_count == 1)
    {
        Log_Event("Displaying latest transaction");

        Log_Message(trans1);
        Log_Message("\r\n");

        CmdLCD(GOTO_LINE2_POS0);
        StrLCD(trans1);

        delay_ms(2000);
    }
    else
    {
        Log_Event("Displaying latest transaction");

        Log_Message(trans1);
        Log_Message("\r\n");

        CmdLCD(GOTO_LINE2_POS0);
        StrLCD(trans1);

        delay_ms(2000);

        CmdLCD(0x01);
        CmdLCD(GOTO_LINE1_POS0);
        StrLCD("Previous Trans:");

        Log_Event("Displaying previous transaction");

        Log_Message(trans2);
        Log_Message("\r\n");

        CmdLCD(GOTO_LINE2_POS0);
        StrLCD(trans2);

        delay_ms(2000);
    }

    Log_Event("Mini statement completed");
}

/* =====================================================
   MAIN MENU
   ===================================================== */

/*
 * menu()
 * Displays the top-level ATM menu and transfers control to Service_Menu().
 * If the PIN restart flag is set, it returns to main() so account
 * verification can begin again.
 */
void menu(void)
{
    while(1)
    {
        CmdLCD(0x01);
        CmdLCD(GOTO_LINE1_POS0);
        StrLCD("1:Services");

        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("2:Exit");

        Log_Event("========== MAIN MENU ==========");
        Log_Event("Waiting for main menu choice");

        while(ColScan());

        choice = KeyScan();

        while(ColScan() == 0);

        //Log_Message("Main menu key received: ");
        //UART_Txstr((s8 *)&choice);
        //Log_Message("\r\n");

        if(choice == '1')
        {
            Log_Event("Services selected");

            Service_Menu();

            /* Return to main() if PIN attempts are exhausted */
            /* Stop menu processing and pass the restart request upward. */
            if(pin_retry_restart == 1)
                return;
        }
        else if(choice == '2')
        {
            Log_Event("========== ATM EXIT ==========");
            Log_Event("Returning to account login");

            CmdLCD(0x01);
            CmdLCD(GOTO_LINE1_POS0);
            StrLCD("Thank You");

            CmdLCD(GOTO_LINE2_POS0);
            StrLCD("Visit Again");

            delay_s(4);

            /* Return to main(); its login loop will ask for the account number. */
            return;
        }
        else
        {
            Log_Event("Invalid main menu choice");

            CmdLCD(0x01);
            StrLCD("Invalid Choice");

            delay_ms(1000);
        }
    }
}

/* =====================================================
   SERVICES MENU
   ===================================================== */

/*
 * Service_Menu()
 * Provides two pages of ATM services.
 * Each protected service calls Pin_Verification() before execution.
 */
void Service_Menu(void)
{
    u8 page = 1;

    Log_Event("Services menu entered");

    while(1)
    {
        CmdLCD(0x01);
        CmdLCD(GOTO_LINE1_POS0);

        if(page == 1)
        {
            StrLCD("1:Balance 2:Dep");

            CmdLCD(GOTO_LINE2_POS0);
            StrLCD("3:Wdraw 4:More");

            Log_Event("Services page 1 displayed");
        }
        else
        {
            StrLCD("1:Change PIN");

            CmdLCD(GOTO_LINE2_POS0);
            StrLCD("2:Mini 3:Back");

            Log_Event("Services page 2 displayed");
        }

        Log_Event("Waiting for service selection");

        while(ColScan());

        choice = KeyScan();

        while(ColScan() == 0);

        Log_Message("Service menu key received: ");
        UART_Txstr((s8 *)&choice);
        Log_Message("\r\n");

        if(page == 1)
        {
            if(choice == '1' || choice == '2' || choice == '3')
            {
                if(choice == '1')
                    Log_Event("Balance enquiry selected");
                else if(choice == '2')
                    Log_Event("Deposit selected");
                else
                    Log_Event("Withdrawal selected");

                stat_chng_pin = 0;

                Pin_Verification();

                /* Do not continue showing services after a restart request. */
                /* Propagate the restart request to menu() and then main(). */
                if(pin_retry_restart == 1)
                    return;
            }
            else if(choice == '4')
            {
                Log_Event("Next services page selected");
                page = 2;
            }
            else
            {
                Log_Event("Invalid services menu choice");

                CmdLCD(0x01);
                StrLCD("Invalid Choice");

                delay_ms(1000);
            }
        }
        else
        {
            if(choice == '1')
            {
                Log_Event("Change PIN selected");

                choice = '4';
                stat_chng_pin = 1;

                Pin_Verification();

                /* Propagate the restart request to menu() and then main(). */
                if(pin_retry_restart == 1)
                    return;
            }
            else if(choice == '2')
            {
                Log_Event("Mini statement selected");

                choice = '5';
                stat_chng_pin = 0;

                Pin_Verification();

                if(pin_retry_restart == 1)
                    return;
            }
            else if(choice == '3')
            {
                Log_Event("Returning to main menu");

                return;
            }
            else
            {
                Log_Event("Invalid services menu choice");

                CmdLCD(0x01);
                StrLCD("Invalid Choice");

                delay_ms(1000);
            }
        }
    }
}

/* =====================================================
   PIN VERIFICATION
   ===================================================== */

/*
 * Pin_Verification()
 * Allows up to three PIN-entry attempts.
 * On success, it runs the selected service.
 * After three failures, it waits three seconds, sets the restart flag
 * and returns through the menu functions to main().
 */
void Pin_Verification(void)
{
    u16 pin_cnt = 0, i;
    u8 flag;
    u8 en_pin[5] = "    ";

    Log_Event("PIN verification started");

    while(pin_cnt < 3)
    {
        i = 0;
        flag = 0;

        memset(en_pin, ' ', 4);
        en_pin[4] = '\0';

        CmdLCD(0x01);
        CmdLCD(GOTO_LINE1_POS0);
        StrLCD("Enter 4-dig Pin:");

        CmdLCD(GOTO_LINE2_POS0);

        Log_Event("Waiting for PIN input");

        while(i < 4)
        {
            while(ColScan());

            en_pin[i] = KeyScan();

            while(ColScan() == 0);

            CmdLCD(GOTO_LINE2_POS0 + i);
            CharLCD('*');

            if(en_pin[i] < '0' || en_pin[i] > '9')
            {
                flag = 1;
                break;
            }

            i++;
        }

        /* Compare only after four valid numeric characters were entered. */
        if(flag == 0 &&
           strcmp((char *)pin, (char *)en_pin) == 0)
        {
            Log_Event("PIN verification successful");

            CmdLCD(0x01);
            StrLCD("Correct PIN");

            delay_ms(500);

            if(stat_chng_pin == 1)
            {
                Change_Pin();
            }

            if(choice == '1')
                Balance_Enquiry();
            else if(choice == '2')
                Deposit();
            else if(choice == '3')
                Withdraw();
            else if(choice == '5')
                Mini_Statement();

            Log_Event("Requested service completed");

            return; /* Exit this void function after the service is complete. */
        }

        pin_cnt++;

        Log_Event("PIN verification failed");

        Log_Message("Attempts remaining: ");
        UART_TxU32(3 - pin_cnt);
        Log_Message("\r\n");

        CmdLCD(0x01);
        CmdLCD(GOTO_LINE1_POS0);
        StrLCD("Incorrect PIN");
		IOSET0=1<<30;
        CmdLCD(GOTO_LINE2_POS0);
        U32LCD(3 - pin_cnt);
        StrLCD(" attempts left");

        delay_ms(1000);
		IOCLR0=1<<30;
    }

    /* Three incorrect attempts completed */

    Log_Event("Maximum incorrect PIN attempts reached");

    CmdLCD(0x01);
    CmdLCD(GOTO_LINE1_POS0);
    StrLCD("Thank You");
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("Try after 24 hrs");

    //Log_Event("ATM restarting after 3 seconds");

    delay_s(3);

    /* Notify main() that account verification must start again. */
    pin_retry_restart = 1;

    return; /* Return control to Service_Menu(), then menu(), then main(). */
}
