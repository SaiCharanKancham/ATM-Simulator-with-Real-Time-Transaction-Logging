
/*
 * File: atm_services.c
 *
 * Description:
 * Contains all ATM application services and menu handling.
 *
 * Main Features:
 * 1. Balance enquiry
 * 2. Deposit and withdrawal
 * 3. PIN change and verification
 * 4. Mini statement
 * 5. LCD menu handling
 * 6. UART event logging
 * 7. Restart after three incorrect PIN attempts
 */


/* ================= HEADER FILES ================= */

#include <LPC214x.h>       // Contains LPC2148 register definitions.
#include <string.h>        // Provides strcpy(), strcmp() and memset().
#include <stdio.h>         // Provides sprintf() function.
#include "lcd.h"           // LCD interfacing functions.
#include "delay.h"         // Delay functions.
#include "lcd_defines.h"   // LCD command and position definitions.
#include "kpm.h"           // Keypad interfacing functions.
#include "types.h"         // User-defined data types.
#include "atm.h"            // ATM function declarations.
#include "uart_log.h"      // UART logging functions.
#include "uart.h"          // UART communication functions.


/* ================= GLOBAL VARIABLES ================= */

/* Stores the four-digit account PIN and null character. */
u8 pin[5] = "2222";

/* Stores the current account balance. */
u32 bal = 1000;

/* Indicates whether the Change PIN service is selected. */
u8 stat_chng_pin = 0;

/* Stores the menu selection entered by the user. */
u8 choice;

/* Becomes 1 when the user enters an incorrect PIN three times. */
u8 pin_retry_restart = 0;

/* Stores the latest transaction. */
char trans1[20] = "";

/* Stores the previous transaction. */
char trans2[20] = "";

/* Stores the number of available transactions (maximum 2). */
u8 trans_count = 0;


/* =====================================================
   TRANSACTION STORAGE
   ===================================================== */

/*
 * Save_Transaction()
 * Stores the latest two transactions.
 *
 * trans1 = Latest transaction.
 * trans2 = Previous transaction.
 */
void Save_Transaction(s8 *type, u32 amount)
{
    /* Copies the latest transaction into the previous transaction variable. */
    strcpy(trans2, trans1);

    /* Combines transaction type and amount into a single string. */
    /* Example: Deposit:500 */
    sprintf(trans1, "%s:%lu",
            (char *)type, (unsigned long)amount);

    /* Increments transaction count until it reaches 2. */
    if(trans_count < 2)
        trans_count++;

    /* Logs that a transaction has been saved. */
    Log_Event("Transaction saved");

    /* Sends transaction type heading through UART. */
    Log_Message("Transaction type: ");

    /* Sends the transaction type through UART. */
    Log_Message((char *)type);

    /* Sends a new line through UART. */
    Log_Message("\r\n");

    /* Sends transaction amount heading through UART. */
    Log_Message("Transaction amount: ");

    /* Sends the transaction amount through UART. */
    Log_Number(amount);

    /* Moves UART output to the next line. */
    Log_Message("\r\n");
}


/* =====================================================
   BALANCE ENQUIRY
   ===================================================== */

/* Displays the current account balance on the LCD. */
void Balance_Enquiry(void)
{
    /* Logs the beginning of balance enquiry. */
    Log_Event("Balance enquiry started");

    /* Clears the LCD display. */
    CmdLCD(0x01);

    /* Moves the cursor to the first row, first column. */
    CmdLCD(GOTO_LINE1_POS0);

    /* Displays the balance heading. */
    StrLCD("Avail Balance:");

    /* Moves the cursor to the second row, first column. */
    CmdLCD(GOTO_LINE2_POS0);

    /* Displays the current balance as an unsigned integer. */
    U32LCD(bal);

    /* Sends the balance heading through UART. */
    Log_Message("Available balance: ");

    /* Sends the current balance through UART. */
    Log_Number(bal);

    /* Sends a new line through UART. */
    Log_Message("\r\n");

    /* Keeps the balance visible for two seconds. */
    delay_ms(2000);

    /* Logs completion of balance enquiry. */
    Log_Event("Balance enquiry completed");
}


/* =====================================================
   DEPOSIT
   ===================================================== */

/* Handles the deposit operation. */
void Deposit(void)
{
    /* Stores the deposit amount entered by the user. */
    u32 amnt_dpt;

    /* Logs the beginning of deposit operation. */
    Log_Event("Deposit transaction started");

    /* Clears the LCD display. */
    CmdLCD(0x01);

    /* Moves the cursor to the first row. */
    CmdLCD(GOTO_LINE1_POS0);

    /* Requests the user to enter the deposit amount. */
    StrLCD("Enter Amnt:");

    /* Logs that the program is waiting for deposit input. */
    Log_Event("Waiting for deposit amount");

    /* Reads the amount entered through the keypad. */
    amnt_dpt = Input();

    /* Sends the entered deposit amount heading through UART. */
    Log_Message("Deposit amount entered: ");

    /* Sends the entered deposit amount through UART. */
    Log_Number(amnt_dpt);

    /* Moves UART output to the next line. */
    Log_Message("\r\n");

    /*
     * Checks whether the deposit amount is invalid.
     *
     * amnt_dpt <= 0:
     * Rejects zero amount.
     *
     * amnt_dpt % 100 != 0:
     * Checks whether the amount is not divisible by 100.
     *
     * amnt_dpt % 500 != 0:
     * Checks whether the amount is not divisible by 500.
     */
    if(amnt_dpt <= 0 ||
       (((amnt_dpt % 100) != 0) &&
        ((amnt_dpt % 500) != 0)))
    {
        /* Logs deposit rejection due to invalid amount. */
        Log_Event("Deposit rejected: invalid amount");

        /* Clears the LCD. */
        CmdLCD(0x01);

        /* Displays invalid amount message. */
        StrLCD("Invalid Amnt");

        /* Keeps the error message visible for one second. */
        delay_ms(1000);
    }
    else
    {
        /* Adds the deposited amount to the existing balance. */
        bal = bal + amnt_dpt;

        /* Clears the LCD. */
        CmdLCD(0x01);

        /* Displays successful deposit message. */
        StrLCD("Success");

        /* Logs successful deposit. */
        Log_Event("Deposit successful");

        /* Sends updated balance heading through UART. */
        Log_Message("Updated balance: ");

        /* Sends the updated balance through UART. */
        Log_Number(bal);

        /* Moves UART output to the next line. */
        Log_Message("\r\n");

        /* Saves the deposit transaction in transaction history. */
        Save_Transaction("Deposit", amnt_dpt);

        /* Keeps success message visible for one second. */
        delay_ms(1000);
    }

    /* Logs completion of deposit function. */
    Log_Event("Deposit function completed");
}


/* =====================================================
   WITHDRAWAL
   ===================================================== */

/* Handles the withdrawal operation. */
void Withdraw(void)
{
    /* Stores the withdrawal amount entered by the user. */
    u32 amnt_wth_drw;

    /* Logs the beginning of withdrawal operation. */
    Log_Event("Withdrawal transaction started");

    /* Clears the LCD. */
    CmdLCD(0x01);

    /* Moves the cursor to the first row. */
    CmdLCD(GOTO_LINE1_POS0);

    /* Requests the user to enter the withdrawal amount. */
    StrLCD("Enter Amnt:");

    /* Logs that the program is waiting for withdrawal input. */
    Log_Event("Waiting for withdrawal amount");

    /* Reads the withdrawal amount through the keypad. */
    amnt_wth_drw = Input();

    /* Sends withdrawal amount heading through UART. */
    Log_Message("Withdrawal amount entered: ");

    /* Sends the entered withdrawal amount through UART. */
    Log_Number(amnt_wth_drw);

    /* Moves UART output to the next line. */
    Log_Message("\r\n");

    /*
     * Checks whether the withdrawal amount is invalid.
     *
     * amnt_wth_drw > bal:
     * Requested amount is greater than available balance.
     *
     * amnt_wth_drw == 0:
     * Requested amount is zero.
     *
     * Remaining condition:
     * Checks the denomination rules.
     */
    if((amnt_wth_drw > bal) ||
       (amnt_wth_drw == 0) ||
       (((amnt_wth_drw % 100) != 0) &&
        ((amnt_wth_drw % 500) != 0)))
    {
        /* Logs withdrawal rejection. */
        Log_Event("Withdrawal rejected");

        /* Checks whether the requested amount exceeds the balance. */
        if(amnt_wth_drw > bal)
        {
            /* Logs insufficient balance reason. */
            Log_Event("Reason: insufficient balance");

            /* Clears the LCD. */
            CmdLCD(0x01);

            /* Displays insufficient balance message. */
            StrLCD("Insuff Bal");

            /* Waits for one second. */
            delay_ms(1000);
        }
        /* Checks whether the requested amount is zero. */
        else if(amnt_wth_drw == 0)
        {
            /* Logs zero amount reason. */
            Log_Event("Reason: zero amount");

            /* Clears the LCD. */
            CmdLCD(0x01);

            /* Displays invalid amount message. */
            StrLCD("Invalid Amnt");

            /* Waits for one second. */
            delay_ms(1000);
        }
        else
        {
            /* Logs invalid denomination reason. */
            Log_Event("Reason: invalid input");

            /* Clears the LCD. */
            CmdLCD(0x01);

            /* Displays invalid amount message. */
            StrLCD("Invalid Amnt");

            /* Waits for one second. */
            delay_ms(1000);
        }
    }
    else
    {
        /* Deducts the withdrawal amount from the account balance. */
        bal = bal - amnt_wth_drw;

        /* Clears the LCD. */
        CmdLCD(0x01);

        /* Displays successful withdrawal message. */
        StrLCD("Success");

        /* Logs successful withdrawal. */
        Log_Event("Withdrawal successful");

        /* Sends updated balance heading through UART. */
        Log_Message("Updated balance: ");

        /* Sends the updated balance through UART. */
        Log_Number(bal);

        /* Moves UART output to the next line. */
        Log_Message("\r\n");

        /* Saves the withdrawal transaction. */
        Save_Transaction("Withdraw", amnt_wth_drw);

        /* Keeps success message visible for one second. */
        delay_ms(1000);
    }

    /* Logs completion of withdrawal function. */
    Log_Event("Withdrawal function completed");
}


/* =====================================================
   CHANGE PIN
   ===================================================== */

/* Allows the user to change the existing four-digit PIN. */
void Change_Pin(void)
{
    /* Index used to store the four PIN digits. */
    u8 i = 0;

    /* Creates an array to store the new PIN. */
    u8 new_pin[5] = "    ";

    /* Logs the beginning of PIN change operation. */
    Log_Event("PIN change started");

    /* Clears the LCD. */
    CmdLCD(0x01);

    /* Moves cursor to the first row. */
    CmdLCD(GOTO_LINE1_POS0);

    /* Requests the user to enter a new four-digit PIN. */
    StrLCD("New 4-dig Pin:");

    /* Logs that the program is waiting for new PIN input. */
    Log_Event("Waiting for new PIN input");

    /* Repeats until four PIN characters are entered. */
    while(i < 4)
    {
        /* Waits until a key is pressed. */
        while(ColScan());

        /* Reads the pressed key from the keypad. */
        new_pin[i] = KeyScan();

        /* Waits until the pressed key is released. */
        while(ColScan() == 0);

        /* Moves the LCD cursor to the position of the current digit. */
        CmdLCD(GOTO_LINE2_POS0 + i);

        /* Displays '*' instead of the actual PIN digit. */
        CharLCD('*');

        /* Moves to the next PIN position. */
        i++;
    }

    /* Checks all four entered PIN characters. */
    for(i = 0; i < 4; i++)
    {
        /* Checks whether the entered character is not a numeric digit. */
        if(new_pin[i] < '0' || new_pin[i] > '9')
        {
            /* Logs PIN change failure. */
            Log_Event("PIN change failed: invalid digit");

            /* Clears the LCD. */
            CmdLCD(0x01);

            /* Displays invalid PIN message. */
            StrLCD("Invalid PIN");

            /* Waits for one second. */
            delay_ms(1000);

            /* Returns to the calling function without changing the PIN. */
            return;
        }
    }

    /* Copies the new PIN into the existing PIN variable. */
    strcpy((char *)pin, (char *)new_pin);

    /* Clears the LCD. */
    CmdLCD(0x01);

    /* Displays PIN updated message. */
    StrLCD("PIN Updated");

    /* Logs successful PIN change. */
    Log_Event("PIN changed successfully");

    /* Waits for one second. */
    delay_ms(1000);

    /* Resets the Change PIN selection flag. */
    stat_chng_pin = 0;

    /* Logs completion of PIN change. */
    Log_Event("PIN change completed");
}


/* =====================================================
   MINI STATEMENT
   ===================================================== */

/* Displays the latest two transactions stored in RAM. */
void Mini_Statement(void)
{
    /* Logs that the user requested a mini statement. */
    Log_Event("Mini statement requested");

    /* Clears the LCD. */
    CmdLCD(0x01);

    /* Moves the cursor to the first row. */
    CmdLCD(GOTO_LINE1_POS0);

    /* Displays mini statement heading. */
    StrLCD("Mini Statement");

    /* Checks whether there are no saved transactions. */
    if(trans_count == 0)
    {
        /* Logs that no transactions are available. */
        Log_Event("No transactions available");

        /* Moves cursor to the second row. */
        CmdLCD(GOTO_LINE2_POS0);

        /* Displays no transactions message. */
        StrLCD("No Transactions");

        /* Waits for 1.5 seconds. */
        delay_ms(1500);
    }
    /* Checks whether exactly one transaction is available. */
    else if(trans_count == 1)
    {
        /* Logs that the latest transaction is being displayed. */
        Log_Event("Displaying latest transaction");

        /* Sends the latest transaction through UART. */
        Log_Message(trans1);

        /* Sends a new line. */
        Log_Message("\r\n");

        /* Moves cursor to the second row. */
        CmdLCD(GOTO_LINE2_POS0);

        /* Displays the latest transaction on the LCD. */
        StrLCD(trans1);

        /* Waits for two seconds. */
        delay_ms(2000);
    }
    else
    {
        /* Logs that the latest transaction is being displayed. */
        Log_Event("Displaying latest transaction");

        /* Sends the latest transaction through UART. */
        Log_Message(trans1);

        /* Sends a new line. */
        Log_Message("\r\n");

        /* Moves cursor to the second row. */
        CmdLCD(GOTO_LINE2_POS0);

        /* Displays the latest transaction. */
        StrLCD(trans1);

        /* Waits for two seconds. */
        delay_ms(2000);

        /* Clears the LCD. */
        CmdLCD(0x01);

        /* Moves cursor to the first row. */
        CmdLCD(GOTO_LINE1_POS0);

        /* Displays previous transaction heading. */
        StrLCD("Previous Trans:");

        /* Logs that the previous transaction is being displayed. */
        Log_Event("Displaying previous transaction");

        /* Sends previous transaction through UART. */
        Log_Message(trans2);

        /* Sends a new line. */
        Log_Message("\r\n");

        /* Moves cursor to the second row. */
        CmdLCD(GOTO_LINE2_POS0);

        /* Displays the previous transaction on the LCD. */
        StrLCD(trans2);

        /* Waits for two seconds. */
        delay_ms(2000);
    }

    /* Logs completion of mini statement. */
    Log_Event("Mini statement completed");
}


/* =====================================================
   MAIN MENU
   ===================================================== */

/* Displays the main ATM menu and handles user selection. */
void menu(void)
{
    /* Creates an infinite loop to keep displaying the main menu. */
    while(1)
    {
        /* Clears the LCD. */
        CmdLCD(0x01);

        /* Moves cursor to the first row. */
        CmdLCD(GOTO_LINE1_POS0);

        /* Displays services option. */
        StrLCD("1:Services");

        /* Moves cursor to the second row. */
        CmdLCD(GOTO_LINE2_POS0);

        /* Displays exit option. */
        StrLCD("2:Exit");

        /* Logs the main menu heading. */
        Log_Event("========== MAIN MENU ==========");

        /* Logs that the program is waiting for menu selection. */
        Log_Event("Waiting for main menu choice");

        /* Waits until the user presses a key. */
        while(ColScan());

        /* Reads the selected key from the keypad. */
        choice = KeyScan();

        /* Waits until the pressed key is released. */
        while(ColScan() == 0);

        /*
         * Checks whether the user selected Services.
         */
        if(choice == '1')
        {
            /* Logs that services option is selected. */
            Log_Event("Services selected");

            /* Calls the services menu function. */
            Service_Menu();

            /* Checks whether PIN attempts are exhausted. */
            if(pin_retry_restart == 1)

                /* Returns to main() to restart account login. */
                return;
        }
        /*
         * Checks whether the user selected Exit.
         */
        else if(choice == '2')
        {
            /* Logs ATM exit heading. */
            Log_Event("========== ATM EXIT ==========");

            /* Logs that control is returning to account login. */
            Log_Event("Returning to account login");

            /* Clears the LCD. */
            CmdLCD(0x01);

            /* Moves cursor to the first row. */
            CmdLCD(GOTO_LINE1_POS0);

            /* Displays thank you message. */
            StrLCD("Thank You");

            /* Moves cursor to the second row. */
            CmdLCD(GOTO_LINE2_POS0);

            /* Displays visit again message. */
            StrLCD("Visit Again");

            /* Displays the message for four seconds. */
            delay_s(4);

            /* Returns to main() to restart account login. */
            return;
        }
        else
        {
            /* Logs invalid menu selection. */
            Log_Event("Invalid main menu choice");

            /* Clears the LCD. */
            CmdLCD(0x01);

            /* Displays invalid choice message. */
            StrLCD("Invalid Choice");

            /* Waits for one second. */
            delay_ms(1000);
        }
    }
}


/* =====================================================
   SERVICES MENU
   ===================================================== */

/* Displays two pages containing different ATM services. */
void Service_Menu(void)
{
    /* Initially selects the first services page. */
    u8 page = 1;

    /* Logs entry into the services menu. */
    Log_Event("Services menu entered");

    /* Keeps displaying the services menu until the user returns. */
    while(1)
    {
        /* Clears the LCD. */
        CmdLCD(0x01);

        /* Moves cursor to the first row. */
        CmdLCD(GOTO_LINE1_POS0);

        /* Checks whether the first services page is selected. */
        if(page == 1)
        {
            /* Displays balance and deposit options. */
            StrLCD("1:Balance 2:Dep");

            /* Moves cursor to the second row. */
            CmdLCD(GOTO_LINE2_POS0);

            /* Displays withdrawal and more options. */
            StrLCD("3:Wdraw 4:More");

            /* Logs that page one is displayed. */
            Log_Event("Services page 1 displayed");
        }
        else
        {
            /* Displays Change PIN option. */
            StrLCD("1:Change PIN");

            /* Moves cursor to the second row. */
            CmdLCD(GOTO_LINE2_POS0);

            /* Displays mini statement and back options. */
            StrLCD("2:Mini 3:Back");

            /* Logs that page two is displayed. */
            Log_Event("Services page 2 displayed");
        }

        /* Logs that the program is waiting for service selection. */
        Log_Event("Waiting for service selection");

        /* Waits until a key is pressed. */
        while(ColScan());

        /* Reads the selected service key. */
        choice = KeyScan();

        /* Waits until the key is released. */
        while(ColScan() == 0);

        /* Logs the selected keypad character through UART. */
        Log_Message("Service menu key received: ");

        /* Sends the selected key through UART. */
        UART_Txstr((s8 *)&choice);

        /* Sends a new line. */
        Log_Message("\r\n");

        /* Checks whether the first services page is active. */
        if(page == 1)
        {
            /* Checks whether balance, deposit or withdrawal is selected. */
            if(choice == '1' || choice == '2' || choice == '3')
            {
                /* Logs balance enquiry selection. */
                if(choice == '1')
                    Log_Event("Balance enquiry selected");

                /* Logs deposit selection. */
                else if(choice == '2')
                    Log_Event("Deposit selected");

                /* Logs withdrawal selection. */
                else
                    Log_Event("Withdrawal selected");

                /* Resets Change PIN selection flag. */
                stat_chng_pin = 0;

                /* Calls PIN verification before accessing the service. */
                Pin_Verification();

                /* Checks whether three incorrect attempts occurred. */
                if(pin_retry_restart == 1)

                    /* Returns control to menu(). */
                    return;
            }
            /* Checks whether the user selected the next page. */
            else if(choice == '4')
            {
                /* Logs next page selection. */
                Log_Event("Next services page selected");

                /* Changes the active page from page one to page two. */
                page = 2;
            }
            else
            {
                /* Logs invalid service selection. */
                Log_Event("Invalid services menu choice");

                /* Clears the LCD. */
                CmdLCD(0x01);

                /* Displays invalid choice message. */
                StrLCD("Invalid Choice");

                /* Waits for one second. */
                delay_ms(1000);
            }
        }
        else
        {
            /* Checks whether Change PIN is selected. */
            if(choice == '1')
            {
                /* Logs Change PIN selection. */
                Log_Event("Change PIN selected");

                /* Assigns service identifier 4 for Change PIN. */
                choice = '4';

                /* Sets flag to indicate Change PIN operation. */
                stat_chng_pin = 1;

                /* Calls PIN verification before changing the PIN. */
                Pin_Verification();

                /* Checks whether three incorrect attempts occurred. */
                if(pin_retry_restart == 1)

                    /* Returns control to menu(). */
                    return;
            }
            /* Checks whether Mini Statement is selected. */
            else if(choice == '2')
            {
                /* Logs mini statement selection. */
                Log_Event("Mini statement selected");

                /* Assigns service identifier 5 for Mini Statement. */
                choice = '5';

                /* Resets Change PIN selection flag. */
                stat_chng_pin = 0;

                /* Calls PIN verification before showing transactions. */
                Pin_Verification();

                /* Checks whether three incorrect attempts occurred. */
                if(pin_retry_restart == 1)

                    /* Returns control to menu(). */
                    return;
            }
            /* Checks whether the user selected Back. */
            else if(choice == '3')
            {
                /* Logs return to main menu. */
                Log_Event("Returning to main menu");

                /* Returns to the main ATM menu. */
                return;
            }
            else
            {
                /* Logs invalid service selection. */
                Log_Event("Invalid services menu choice");

                /* Clears the LCD. */
                CmdLCD(0x01);

                /* Displays invalid choice message. */
                StrLCD("Invalid Choice");

                /* Waits for one second. */
                delay_ms(1000);
            }
        }
    }
}


/* =====================================================
   PIN VERIFICATION
   ===================================================== */

/* Verifies the user's PIN before executing the selected service. */
void Pin_Verification(void)
{
    /* Counts the number of incorrect PIN attempts. */
    u16 pin_cnt = 0, i;

    /* Indicates whether an invalid character was entered. */
    u8 flag;

    /* Stores the PIN entered by the user. */
    u8 en_pin[5] = "    ";

    /* Logs the beginning of PIN verification. */
    Log_Event("PIN verification started");

    /* Allows a maximum of three PIN attempts. */
    while(pin_cnt < 3)
    {
        /* Resets the input index for every new attempt. */
        i = 0;

        /* Resets the invalid input flag. */
        flag = 0;

        /* Initializes the first four positions with spaces. */
        memset(en_pin, ' ', 4);

        /* Places null character at the end of the entered PIN. */
        en_pin[4] = '\0';

        /* Clears the LCD. */
        CmdLCD(0x01);

        /* Moves cursor to the first row. */
        CmdLCD(GOTO_LINE1_POS0);

        /* Requests the user to enter the four-digit PIN. */
        StrLCD("Enter 4-dig Pin:");

        /* Moves cursor to the second row. */
        CmdLCD(GOTO_LINE2_POS0);

        /* Logs that the program is waiting for PIN input. */
        Log_Event("Waiting for PIN input");

        /* Repeats until four PIN characters are entered. */
        while(i < 4)
        {
            /* Waits until a key is pressed. */
            while(ColScan());

            /* Reads the pressed key and stores it in the PIN array. */
            en_pin[i] = KeyScan();

            /* Waits until the pressed key is released. */
            while(ColScan() == 0);

            /* Moves LCD cursor to the current PIN position. */
            CmdLCD(GOTO_LINE2_POS0 + i);

            /* Displays '*' instead of the actual PIN character. */
            CharLCD('*');

            /* Checks whether the entered character is not numeric. */
            if(en_pin[i] < '0' || en_pin[i] > '9')
            {
                /* Sets flag to indicate invalid input. */
                flag = 1;

                /* Stops reading further PIN characters. */
                break;
            }

            /* Moves to the next PIN position. */
            i++;
        }

        /*
         * Checks two conditions:
         *
         * flag == 0:
         * All entered characters are numeric.
         *
         * strcmp() == 0:
         * Entered PIN matches the stored PIN.
         */
        if(flag == 0 &&
           strcmp((char *)pin, (char *)en_pin) == 0)
        {
            /* Logs successful PIN verification. */
            Log_Event("PIN verification successful");

            /* Clears the LCD. */
            CmdLCD(0x01);

            /* Displays correct PIN message. */
            StrLCD("Correct PIN");

            /* Waits for half a second. */
            delay_ms(500);

            /* Checks whether the selected service is Change PIN. */
            if(stat_chng_pin == 1)
            {
                /* Calls the function to change the existing PIN. */
                Change_Pin();
            }

            /* Checks whether Balance Enquiry is selected. */
            if(choice == '1')
                Balance_Enquiry();

            /* Checks whether Deposit is selected. */
            else if(choice == '2')
                Deposit();

            /* Checks whether Withdrawal is selected. */
            else if(choice == '3')
                Withdraw();

            /* Checks whether Mini Statement is selected. */
            else if(choice == '5')
                Mini_Statement();

            /* Logs completion of the requested service. */
            Log_Event("Requested service completed");

            /* Returns to the calling services menu. */
            return;
        }

        /* Increments the incorrect PIN attempt counter. */
        pin_cnt++;

        /* Logs unsuccessful PIN verification. */
        Log_Event("PIN verification failed");

        /* Sends remaining attempts heading through UART. */
        Log_Message("Attempts remaining: ");

        /* Calculates and sends the remaining attempts. */
        UART_TxU32(3 - pin_cnt);

        /* Sends a new line through UART. */
        Log_Message("\r\n");

        /* Clears the LCD. */
        CmdLCD(0x01);

        /* Moves cursor to the first row. */
        CmdLCD(GOTO_LINE1_POS0);

        /* Displays incorrect PIN message. */
        StrLCD("Incorrect PIN");

        /* Sets GPIO P0.30 HIGH. */
        IOSET0 = 1 << 30;

        /* Moves cursor to the second row. */
        CmdLCD(GOTO_LINE2_POS0);

        /* Displays the number of remaining attempts. */
        U32LCD(3 - pin_cnt);

        /* Displays attempts left message. */
        StrLCD(" attempts left");

        /* Displays the error message for one second. */
        delay_ms(1000);

        /* Sets GPIO P0.30 LOW. */
        IOCLR0 = 1 << 30;
    }

    /* Reaches here after all three PIN attempts are unsuccessful. */

    /* Logs that the maximum incorrect attempts have been reached. */
    Log_Event("Maximum incorrect PIN attempts reached");

    /* Clears the LCD. */
    CmdLCD(0x01);

    /* Moves cursor to the first row. */
    CmdLCD(GOTO_LINE1_POS0);

    /* Displays thank you message. */
    StrLCD("Thank You");

    /* Moves cursor to the second row. */
    CmdLCD(GOTO_LINE2_POS0);

    /* Displays the retry message. */
    StrLCD("Try after 24 hrs");

    /* Waits for three seconds before requesting a restart. */
    delay_s(3);

    /* Sets restart flag to notify main() about PIN failure. */
    pin_retry_restart = 1;

    /* Returns to Service_Menu(), then menu(), and finally main(). */
    return;
}
