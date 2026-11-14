
/*
 * File: main.c
 *
 * Description:
 * Initializes the ATM peripherals, confirms RTC date and time,
 * verifies the account number and starts the ATM menu.
 *
 * If three incorrect PIN attempts occur, the service module sets
 * pin_retry_restart. The program returns here and starts account
 * verification again without resetting the microcontroller.
 */


/* ================= HEADER FILES ================= */

#include <LPC214x.h>       // Contains LPC2148 hardware register definitions.
#include <string.h>        // Provides string functions such as strcmp().
#include <stdio.h>         // Provides sprintf() for formatted strings.

#include "lcd.h"           // LCD interfacing functions.
#include "lcd_defines.h"   // LCD commands and cursor positions.
#include "rtc.h"           // RTC initialization and register definitions.
#include "rtc_edit.h"      // Functions to edit RTC date and time.
#include "kpm.h"           // Keypad interfacing functions.
#include "delay.h"         // Software delay functions.
#include "atm.h"           // ATM functions and global declarations.
#include "uart.h"          // UART communication functions.
#include "uart_log.h"      // UART event logging functions.


/* ================= GLOBAL VARIABLES ================= */

/* Stores the demo account number as five digits and a null character. */
u8 acc_no[6] = "11111";

/* Stores the EINT1 interrupt request flag.
   volatile prevents the compiler from assuming this variable never changes
   outside normal program execution. */
volatile u16 stat_eint1;


/* =====================================================
   EXTERNAL INTERRUPT 1 ISR
   ===================================================== */

/*
 * EINT1_ISR()
 * Interrupt Service Routine executed when EINT1 is triggered.
 */
void EINT1_ISR(void) __irq
{
    /* Sets the flag to inform main() that EINT1 occurred. */
    stat_eint1 = 1;

    /* Clears the EINT1 interrupt flag by writing 1 to EXTINT bit 1. */
    EXTINT = (1 << 1);

    /* Informs the VIC that interrupt servicing is completed. */
    VICVectAddr = 0;
}


/* =====================================================
   EINT1 INITIALIZATION
   ===================================================== */

/*
 * Init_Eint1()
 * Configures P0.3 as EINT1.
 * Enables falling-edge-triggered external interrupt.
 */
void Init_Eint1(void)
{
    /* Clears PINSEL0 bits 6 and 7 to remove the previous pin function. */
    PINSEL0 &= ~(3 << (3 * 2));

    /* Selects function 11 for P0.3, which is EINT1. */
    PINSEL0 |= (3 << (3 * 2));

    /* Configures interrupt 15 as an IRQ interrupt instead of FIQ. */
    VICIntSelect &= ~(1 << 15);

    /* Clears any previously pending EINT1 interrupt. */
    EXTINT = (1 << 1);

    /* Stores the EINT1 ISR address in VIC vector slot 0. */
    VICVectAddr0 = (u32)EINT1_ISR;

    /* Enables vector slot 0 and associates it with interrupt number 15. */
    VICVectCntl0 = (1 << 5) | 15;

    /* Enables external interrupt 1 in the VIC. */
    VICIntEnable |= (1 << 15);

    /* Configures EINT1 to generate an interrupt on an edge transition. */
    EXTMODE |= (1 << 1);

    /* Selects falling-edge triggering for EINT1. */
    EXTPOLAR &= ~(1 << 1);
}


/* =====================================================
   ACCOUNT NUMBER VERIFICATION
   ===================================================== */

/*
 * Accnt_Verification()
 * Reads a five-digit account number from the keypad.
 *
 * First two digits are displayed normally.
 * Remaining three digits are displayed as '*'.
 *
 * Returns:
 * 1 = Account number matched.
 * 0 = Invalid or non-matching account number.
 */
u16 Accnt_Verification(void)
{
    /* Creates an array to store the entered account number. */
    u8 ent_acc_no[6] = "     ";

    /* Index used to store the five account number digits. */
    u16 i = 0;

    /* Clears the LCD display. */
    CmdLCD(0x01);

    /* Moves cursor to the first row. */
    CmdLCD(GOTO_LINE1_POS0);

    /* Displays account number input message. */
    StrLCD("A/c No(5-dig):");

    /* Repeats until five account number characters are entered. */
    while(i < 5)
    {
        /* Waits until a key is pressed. */
        while(ColScan());

        /* Reads the pressed key and stores it in the array. */
        ent_acc_no[i] = KeyScan();

        /* Waits until the pressed key is released. */
        while(ColScan() == 0);

        /* Checks whether the entered character is between '0' and '9'. */
        if(!(ent_acc_no[i] >= '0' && ent_acc_no[i] <= '9'))
        {
            /* Returns 0 immediately if a non-numeric character is entered. */
            return 0;
        }
        else
        {
            /* Moves the LCD cursor to the current digit position. */
            CmdLCD(GOTO_LINE2_POS0 + i);

            /* Displays the first two account number digits normally. */
            if(i < 2)
                CharLCD(ent_acc_no[i]);

            /* Masks the remaining three digits with '*'. */
            else
                CharLCD('*');

            /* Moves to the next array position. */
            i++;
        }
    }

    /* Compares the entered account number with the stored account number. */
    if(strcmp((char *)ent_acc_no, (char *)acc_no) == 0)
    {
        /* Clears the LCD. */
        CmdLCD(0x01);

        /* Moves cursor to the first row. */
        CmdLCD(GOTO_LINE1_POS0);

        /* Displays account found message. */
        StrLCD("A/c found");

        /* Keeps the message visible for two seconds. */
        delay_s(2);

        /* Returns 1 when both account numbers match. */
        return 1;
    }

    /* Returns 0 when the account numbers do not match. */
    return 0;
}


/* =====================================================
   DISPLAY DATE AND TIME
   ===================================================== */

/*
 * Disp_Date_Time()
 * Reads the current RTC values, formats them and displays them on LCD.
 */
void Disp_Date_Time(void)
{
    /* Creates an array to store the formatted date. */
    u8 date[16];

    /* Creates an array to store the formatted time. */
    u8 time[16];

    /* Formats day, month and year into DD-MM-YYYY format. */
    sprintf((char *)date, "Date:%02d-%02d-%04d",
            (u16)DOM, (u16)MONTH, (u16)YEAR);

    /* Formats hour, minute and second into HH:MM:SS format. */
    sprintf((char *)time, "Time:%02d:%02d:%02d",
            (u16)HOUR, (u16)MIN, (u16)SEC);

    /* Clears the LCD. */
    CmdLCD(0x01);

    /* Moves cursor to the first row. */
    CmdLCD(GOTO_LINE1_POS0);

    /* Displays the formatted date. */
    StrLCD((char *)date);

    /* Moves cursor to the second row. */
    CmdLCD(GOTO_LINE2_POS0);

    /* Displays the formatted time. */
    StrLCD((char *)time);

    /* Keeps date and time visible for two seconds. */
    delay_s(2);
}


/* =====================================================
   ATM WELCOME SCREEN
   ===================================================== */

/*
 * ATM_Welcome_Screen()
 * Displays the ATM welcome message and current RTC values.
 */
void ATM_Welcome_Screen(void)
{
    /* Moves cursor to the first row. */
    CmdLCD(GOTO_LINE1_POS0);

    /* Displays the welcome message. */
    StrLCD("....Welcome....");

    /* Moves cursor to the second row. */
    CmdLCD(GOTO_LINE2_POS0);

    /* Displays ATM Simulator name. */
    StrLCD(" ATM Simulator ");

    /* Keeps the welcome message visible for one second. */
    delay_s(1);

    /* Displays the current RTC date and time. */
    Disp_Date_Time();
}


/* =====================================================
   MAIN FUNCTION
   ===================================================== */

/*
 * main()
 * Initializes peripherals and manages:
 * 1. RTC confirmation.
 * 2. Account number verification.
 * 3. ATM menu execution.
 * 4. Restart after incorrect PIN attempts.
 */
int main(void)
{
    /* Stores the user's RTC confirmation key. */
    u8 key;

    /* Stores the user's RTC editing selection. */
    u8 edit_choice;

    /* Indicates whether the RTC date and time are confirmed. */
    u8 rtc_confirmed = 0;


    /* ================= GPIO CONFIGURATION ================= */

    /* Configures P0.30 as an output pin for indicating the status of incorrect pin */
    IODIR0 = IODIR0 | (1 << 30);


    /* ================= PERIPHERAL INITIALIZATION ================= */

    /* Initializes the LCD. */
    InitLCD();

    /* Initializes the RTC. */
    Init_RTC();

    /* Initializes the keypad. */
    InitKPM();

    /* Configures EINT1. */
    Init_Eint1();

    /* Initializes UART communication. */
    UART_Init();


    /* ================= INITIALIZATION LOGGING ================= */

    /* Logs that the ATM application has started. */
    Log_Event("========== ATM STARTED ==========");

    /* Logs LCD initialization. */
    Log_Event("LCD initialized");

    /* Logs RTC initialization. */
    Log_Event("RTC initialized");

    /* Logs keypad initialization. */
    Log_Event("Keypad initialized");

    /* Logs external interrupt initialization. */
    Log_Event("External interrupt initialized");

    /* Logs UART initialization. */
    Log_Event("UART initialized");


    /* ================= WELCOME SCREEN ================= */

    /* Displays the welcome screen and RTC date/time. */
    ATM_Welcome_Screen();

    /* Logs welcome screen completion. */
    Log_Event("Welcome screen displayed");


    /* ================= RTC CONFIRMATION ================= */

    /* Logs RTC confirmation section heading. */
    Log_Event("========== RTC CONFIRMATION ==========");

    /* Continues until the user confirms the RTC values. */
    while(rtc_confirmed == 0)
    {
        /* Displays the current RTC date and time. */
        Disp_Date_Time();

        /* Clears the LCD. */
        CmdLCD(0x01);

        /* Moves cursor to the first row. */
        CmdLCD(GOTO_LINE1_POS0);

        /* Displays the correct date/time option. */
        StrLCD("1:Correct");

        /* Moves cursor to the second row. */
        CmdLCD(GOTO_LINE2_POS0);

        /* Displays EINT1 editing instruction. */
        StrLCD("EINT1:Edit");

        /* Logs that the program is waiting for RTC confirmation. */
        Log_Event("Waiting for RTC confirmation or EINT1");


        /*
         * Waits until either:
         * 1. EINT1 interrupt occurs.
         * 2. A keypad key is pressed.
         *
         * stat_eint1 == 0 means no EINT1 interrupt occurred.
         * ColScan() != 0 means no key is currently pressed.
         */
        while((stat_eint1 == 0) && (ColScan() != 0))
        {
            /* Remains here until one of the two events occurs. */
        }


        /* Checks whether EINT1 interrupt occurred. */
        if(stat_eint1 == 1)
        {

            /* Clears the EINT1 request flag after detecting the event. */
            stat_eint1 = 0;

            /* Logs that RTC editing was requested using EINT1. */
            Log_Event("RTC edit requested through EINT1");

            /* Clears the LCD. */
            CmdLCD(0x01);

            /* Moves cursor to the first row. */
            CmdLCD(GOTO_LINE1_POS0);

            /* Displays date editing option. */
            StrLCD("1:Date");

            /* Moves cursor to the second row. */
            CmdLCD(GOTO_LINE2_POS0);

            /* Displays time editing option. */
            StrLCD("2:Time");

            /* Logs that the program is waiting for edit selection. */
            Log_Event("Waiting for RTC edit choice");


            /* Waits until the user presses a keypad key. */
            while(ColScan());

            /* Reads the selected RTC editing option. */
            edit_choice = KeyScan();

            /* Waits until the key is released. */
            while(ColScan() == 0);


            /* Checks whether the user selected date editing. */
            if(edit_choice == '1')
            {
                /* Logs date editing selection. */
                Log_Event("Date edit selected");

                /* Calls the function to edit RTC date. */
                Edit_Date();

                /* Displays the updated RTC date and time. */
                Disp_Date_Time();

                /* Waits for two seconds. */
                delay_s(2);

                /* Logs completion of date editing. */
                Log_Event("Date edit completed");
            }

            /* Checks whether the user selected time editing. */
            else if(edit_choice == '2')
            {
                /* Logs time editing selection. */
                Log_Event("Time edit selected");

                /* Calls the function to edit RTC time. */
                Edit_Time();

                /* Displays the updated RTC date and time. */
                Disp_Date_Time();

                /* Waits for two seconds. */
                delay_s(2);

                /* Logs completion of time editing. */
                Log_Event("Time edit completed");
            }
            else
            {
                /* Logs invalid RTC editing selection. */
                Log_Event("Invalid RTC edit choice");
            }


            /* Clears the LCD. */
            CmdLCD(0x01);

            /* Moves cursor to the first row. */
            CmdLCD(GOTO_LINE1_POS0);

            /* Displays correct option again. */
            StrLCD("1:Correct");

            /* Moves cursor to the second row. */
            CmdLCD(GOTO_LINE2_POS0);

            /* Displays EINT1 editing option again. */
            StrLCD("eint:Edit");

            /* Logs that the program is waiting for confirmation. */
            Log_Event("Waiting for RTC confirmation choice");


            /*
             * Waits for either:
             * 1. Another EINT1 interrupt.
             * 2. A keypad key press.
             */
            while((stat_eint1 == 0) && (ColScan() != 0))
            {
                /* Waits for an interrupt or keypad input. */
            }


            /* Checks whether EINT1 occurred again. */
            if(stat_eint1 == 1)
            {
                /* Logs another RTC editing request. */
                Log_Event("RTC edit requested again");
            }
            else
            {
                /* Reads the keypad confirmation key. */
                key = KeyScan();

                /* Waits until the pressed key is released. */
                while(ColScan() == 0);

                /* Checks whether the user confirmed RTC values. */
                if(key == '1')
                {
                    /* Sets RTC confirmation flag to 1. */
                    rtc_confirmed = 1;

                    /* Logs successful RTC confirmation. */
                    Log_Event("RTC confirmed successfully");
                }
                else
                {
                    /* Logs invalid RTC confirmation selection. */
                    Log_Event("Invalid RTC confirmation choice");
                }
            }
        }
        else
        {
            /* Direct RTC confirmation without editing. */

            /* Reads the pressed keypad key. */
            key = KeyScan();

            /* Waits until the pressed key is released. */
            while(ColScan() == 0);

            /* Checks whether the user selected Correct. */
            if(key == '1')
            {
                /* Sets the RTC confirmation flag. */
                rtc_confirmed = 1;

                /* Logs successful RTC confirmation. */
                Log_Event("RTC confirmed successfully");
            }
            else
            {
                /* Logs invalid confirmation selection. */
                Log_Event("Invalid RTC confirmation choice");
            }
        }
    }


    /* ================= RTC CONFIRMED ================= */

    /* Clears the LCD. */
    CmdLCD(0x01);

    /* Moves cursor to the first row. */
    CmdLCD(GOTO_LINE1_POS0);

    /* Displays RTC confirmed message. */
    StrLCD("RTC Confirmed");

    /* Logs completion of RTC confirmation. */
    Log_Event("RTC confirmation completed");

    /* Keeps confirmation message visible for two seconds. */
    delay_s(2);


    /* ================= ACCOUNT LOGIN ================= */

    /* Logs account login section heading. */
    Log_Event("========== ACCOUNT LOGIN ==========");

    /* Infinite loop keeps the ATM ready for another account login. */
    while(1)
    {
        /* Logs that account verification is about to begin. */
        Log_Event("Waiting for account verification");


        /* Calls account verification and checks its return value. */
        if(Accnt_Verification() == 1)
        {
            /* Logs successful account verification. */
            Log_Event("Account verification successful");

            /* Logs RTC date and time information through UART. */
            Log_data();

            /* Logs successful login. */
            Log_Event("Login Success");


            /* Calls the main ATM menu after account verification succeeds. */
            menu();


            /*
             * Checks whether three incorrect PIN attempts occurred.
             * The flag is set inside Pin_Verification() in atm_services.c.
             */
            if(pin_retry_restart == 1)
            {
                /* Clears the restart flag for the next login session. */
                pin_retry_restart = 0;

                /* Logs that account verification is restarting. */
                Log_Event("Restarting from account verification");

                /* Skips remaining statements and starts the next loop iteration. */
                continue;
            }

            /* Logs return to account verification after normal menu Exit. */
            Log_Event("Returning to account verification");
        }
        else
        {
            /* Logs unsuccessful account verification. */
            Log_Event("Account verification failed");

            /* Clears the LCD. */
            CmdLCD(0x01);

            /* Moves cursor to the first row. */
            CmdLCD(GOTO_LINE1_POS0);

            /* Displays account not found message. */
            StrLCD("Accnt Not Found");

            /* Logs the RTC information through UART. */
            Log_data();

            /* Logs unsuccessful login. */
            Log_Event("Login Failed");

            /* Keeps error message visible for two seconds. */
            delay_s(2);
        }
    }
}
