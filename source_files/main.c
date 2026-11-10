/*
 * File: main.c
 * Description:
 *     Initializes the ATM peripherals, confirms the RTC date/time,
 *     verifies the account number and starts the ATM menu.
 *
 *     If three incorrect PIN attempts occur, the service module sets
 *     pin_retry_restart. The program then returns here and starts
 *     account-number verification again without resetting the MCU.
 */

#include <LPC21xx.h>
#include <string.h>
#include <stdio.h>

#include "lcd.h"
#include "lcd_defines.h"
#include "rtc.h"
#include "rtc_edit.h"
#include "kpm.h"
#include "delay.h"
#include "atm.h"
#include "uart.h"
#include "uart_log.h"

/* Demo account number used by this ATM application. */
u8 acc_no[6] = "11111";

/* Set by the EINT1 interrupt when the user requests RTC editing. */
volatile u16 stat_eint1;

/*
 * EINT1 interrupt service routine.
 * Sets a request flag, clears the external interrupt and informs the VIC
 * that the interrupt service routine has completed.
 */
void EINT1_ISR(void) __irq
{
    stat_eint1 = 1;
    EXTINT = (1 << 1);
    VICVectAddr = 0;
}

/*
 * Init_Eint1()
 * Configures P0.3 as EINT1 and enables a falling-edge-triggered interrupt.
 */
void Init_Eint1(void)
{
    PINSEL0 &= ~(3 << (3 * 2));
    PINSEL0 |= (3 << (3 * 2));

    VICIntSelect &= ~(1 << 15);
    EXTINT = (1 << 1);

    VICVectAddr0 = (u32)EINT1_ISR;
    VICVectCntl0 = (1 << 5) | 15;

    VICIntEnable |= (1 << 15);

    EXTMODE |= (1 << 1);
    EXTPOLAR &= ~(1 << 1);
}

/*
 * Accnt_Verification()
 * Reads a five-digit account number from the keypad.
 * The first two digits are displayed normally and the remaining digits
 * are masked. Returns 1 for a match and 0 for an invalid/non-matching ID.
 */
u16 Accnt_Verification(void)
{
    u8 ent_acc_no[6] = "     ";
    u16 i = 0;

    CmdLCD(0x01);
    CmdLCD(GOTO_LINE1_POS0);
    StrLCD("A/c No(5-dig):");

    while(i < 5)
    {
        /* Wait until a key is pressed, then read and wait for release. */
        while(ColScan());

        ent_acc_no[i] = KeyScan();

        while(ColScan() == 0);

        /* Account number input must contain decimal digits only. */
        if(!(ent_acc_no[i] >= '0' && ent_acc_no[i] <= '9'))
        {
            return 0;
        }
        else
        {
            CmdLCD(GOTO_LINE2_POS0 + i);

            if(i < 2)
                CharLCD(ent_acc_no[i]);
            else
                CharLCD('*');

            i++;
        }
    }

    /* Compare the entered five-digit number with the stored account number. */
    if(strcmp((char *)ent_acc_no, (char *)acc_no) == 0)
    {
        CmdLCD(0x01);
        CmdLCD(GOTO_LINE1_POS0);
        StrLCD("A/c found");

        delay_s(2);
        return 1;
    }

    return 0;
}

/*
 * Disp_Date_Time()
 * Formats the RTC values and displays the current date and time.
 */
void Disp_Date_Time(void)
{
    u8 date[16], time[16];

    sprintf((char *)date, "Date:%02d-%02d-%04d",
            (u16)DOM, (u16)MONTH, (u16)YEAR);

    sprintf((char *)time, "Time:%02d:%02d:%02d",
            (u16)HOUR, (u16)MIN, (u16)SEC);

    CmdLCD(0x01);
    CmdLCD(GOTO_LINE1_POS0);
    StrLCD((char *)date);

    CmdLCD(GOTO_LINE2_POS0);
    StrLCD((char *)time);

    delay_s(2);
}

/*
 * ATM_Welcome_Screen()
 * Displays the welcome message followed by the current RTC values.
 */
void ATM_Welcome_Screen(void)
{
    CmdLCD(GOTO_LINE1_POS0);
    StrLCD("....Welcome....");

    CmdLCD(GOTO_LINE2_POS0);
    StrLCD(" ATM Simulator ");

    delay_s(1);
	//while(1);
    Disp_Date_Time();
}

/*
 * main()
 * Initializes the peripherals, confirms the RTC and repeatedly performs
 * account verification. A PIN-restart request returns here.
 */
int main(void)
{
    u8 key, edit_choice;
    u8 rtc_confirmed = 0;

    /* Configure P0.30 as an output, as used by this project. */
    IODIR0 = IODIR0 | (1 << 30);

    /* Initialize all peripherals used by the ATM application. */
    InitLCD();
    Init_RTC();
    InitKPM();
    Init_Eint1();
    UART_Init();

    Log_Event("========== ATM STARTED ==========");
    Log_Event("LCD initialized");
    Log_Event("RTC initialized");
    Log_Event("Keypad initialized");
    Log_Event("External interrupt initialized");
    Log_Event("UART initialized");

    ATM_Welcome_Screen();

    Log_Event("Welcome screen displayed");

    /* Keep asking for RTC confirmation until the user accepts the values. */
    Log_Event("========== RTC CONFIRMATION ==========");

    while(rtc_confirmed == 0)
    {
        Disp_Date_Time();

        CmdLCD(0x01);
        CmdLCD(GOTO_LINE1_POS0);
        StrLCD("1:Correct");
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("EINT1:Edit");

        Log_Event("Waiting for RTC confirmation or EINT1");

        /*
         * Wait until either a keypad key is pressed or EINT1 occurs.
         * In this keypad driver, ColScan() returns 0 when a key is pressed.
         */
        while((stat_eint1 == 0) && (ColScan() != 0));

        if(stat_eint1 == 1)
        {
            /* EINT1 has priority if it was the event that ended the wait. */
            label:
			stat_eint1 = 0;

            Log_Event("RTC edit requested through EINT1");

            CmdLCD(0x01);
            CmdLCD(GOTO_LINE1_POS0);
            StrLCD("1:Date");
            CmdLCD(GOTO_LINE2_POS0);
            StrLCD("2:Time");
            Log_Event("Waiting for RTC edit choice");

            while(ColScan());
            edit_choice = KeyScan();
            while(ColScan() == 0);

            if(edit_choice == '1')
            {
                Log_Event("Date edit selected");
                Edit_Date();
				Disp_Date_Time();
				delay_s(2);
                Log_Event("Date edit completed");
				
            }
            else if(edit_choice == '2')
            {
                Log_Event("Time edit selected");
                Edit_Time();
				Disp_Date_Time();
				delay_s(2);
                Log_Event("Time edit completed");
            }
            else
            {
                Log_Event("Invalid RTC edit choice");
            }

            //Disp_Date_Time();

            CmdLCD(0x01);
            CmdLCD(GOTO_LINE1_POS0);
            StrLCD("1:Correct");
            CmdLCD(GOTO_LINE2_POS0);
            StrLCD("eint:Edit");
            Log_Event("Waiting for RTC confirmation choice");
			
			while((stat_eint1 == 0) && (ColScan()!= 0));
			
			if(stat_eint1==1)
			{
			Log_Event("RTC edit requested again");
			goto label;
			}
			else
			{
            key = KeyScan();
            while(ColScan()== 0);
            if(key == '1')
            {
                rtc_confirmed = 1;
                Log_Event("RTC confirmed successfully");
            }
            else
            {
                Log_Event("Invalid RTC confirmation choice");
            }
			}
        }
        else
        {
            /* Direct confirmation: press 1 when the displayed RTC is correct. */
            key = KeyScan();

            while(ColScan() == 0);

            if(key == '1')
            {
                rtc_confirmed = 1;
                Log_Event("RTC confirmed successfully");
            }
            else
            {
                Log_Event("Invalid RTC confirmation choice");
            }
        }
    }

    CmdLCD(0x01);
    CmdLCD(GOTO_LINE1_POS0);
    StrLCD("RTC Confirmed");

    Log_Event("RTC confirmation completed");

    delay_s(2);

    /* Account login loop: repeat after Exit or a PIN-restart request. */
    Log_Event("========== ACCOUNT LOGIN ==========");

    while(1)
    {
        Log_Event("Waiting for account verification");

        if(Accnt_Verification() == 1)
        {
            Log_Event("Account verification successful");

            Log_data();

            Log_Event("Login Success");

            /* Enter the ATM menus only after account verification succeeds. */
            menu();

            /*
             * Three incorrect PIN attempts set this flag in atm_services.c.
             * menu() returns here, and the next loop iteration asks for the
             * account number again. The flag is cleared for the next login.
             */
            if(pin_retry_restart == 1)
            {
                pin_retry_restart = 0;

                Log_Event("Restarting from account verification");

                continue;
            }

            /* Normal menu Exit also returns here and begins another login. */
            Log_Event("Returning to account verification");
        }
        else
        {
            Log_Event("Account verification failed");

            CmdLCD(0x01);
            CmdLCD(GOTO_LINE1_POS0);
            StrLCD("Accnt Not Found");

            Log_data();

            Log_Event("Login Failed");

            delay_s(2);
        }
    }
}
