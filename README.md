# ATM-Simulator-with-Real-Time-Transaction-Logging
# ATM Simulator with Real-Time Transaction Logging

An embedded ATM simulation project developed using the **ARM7 LPC2148 microcontroller**, designed to perform basic ATM operations with PIN-based authentication, RTC-based date and time management, and UART-based real-time transaction logging.

The project demonstrates practical implementation of embedded C programming, peripheral interfacing, interrupt handling, modular driver development, and hardware-level system integration.

---

## 📌 Table of Contents

1. [Project Overview](#-project-overview)
2. [Project Objectives](#-project-objectives)
3. [Hardware Requirements](#-hardware-requirements)
4. [Software Requirements](#-software-requirements)
5. [System Architecture](#-system-architecture)
6. [Hardware Interfacing](#-hardware-interfacing)
7. [Features](#-features)
8. [ATM Operations](#-atm-operations)
9. [System Working Flow](#-system-working-flow)
10. [PIN Authentication and Security](#-pin-authentication-and-security)
11. [RTC-Based Date and Time Management](#-rtc-based-date-and-time-management)
12. [Real-Time Transaction Logging](#-real-time-transaction-logging)
13. [Interrupt Handling](#-interrupt-handling)
14. [Software Architecture](#-software-architecture)
15. [Project Directory Structure](#-project-directory-structure)
16. [Embedded Concepts Implemented](#-embedded-concepts-implemented)
17. [Simulation and Testing](#-simulation-and-testing)
18. [Applications](#-applications)
19. [Limitations](#-limitations)
20. [Future Improvements](#-future-improvements)
21. [Learning Outcomes](#-learning-outcomes)
22. [Author](#-author)

---

## 📖 Project Overview

The ATM Simulator is an embedded systems project that replicates the basic functionality of an Automated Teller Machine using an ARM7-based LPC2148 microcontroller.

The system provides an interactive interface through a 16×2 LCD and a 4×4 matrix keypad. Users can authenticate themselves using an account number and PIN before accessing ATM services.

The system supports balance enquiry, cash withdrawal, cash deposit, PIN modification, and transaction history.

An onboard Real-Time Clock (RTC) maintains date and time information. Transaction details are transmitted through UART, allowing transaction activities to be monitored through a serial terminal.

## 🎯 Project Objectives

- Develop an ATM simulation using an ARM7 microcontroller.
- Implement account and PIN-based authentication.
- Interface LCD and matrix keypad with LPC2148.
- Implement different ATM banking operations.
- Maintain date and time using the internal RTC.
- Record transaction details through UART communication.
- Implement interrupt-based date and time modification.
- Develop reusable peripheral drivers and modular application code.
- Understand embedded system integration through hardware simulation.

## 🔧 Hardware Requirements

| Component | Purpose |
|---|---|
| LPC2148 | ARM7 microcontroller |
| 16×2 LCD | Displaying ATM messages |
| 4×4 Matrix Keypad | User input |
| RTC | Maintaining date and time |
| UART | Serial transaction logging |
| Computer | Simulation and serial monitoring |

## 💻 Software Requirements

| Software | Purpose |
|---|---|
| Keil µVision | Embedded C development |
| Proteus | Circuit simulation |
| GCC / ARM toolchain | ARM compilation (where applicable) |
| Serial Terminal | Monitoring UART output |

## 🏗️ System Architecture

The project follows a modular embedded software architecture. Each peripheral and application functionality is separated into dedicated modules.

**System architecture:**

```text
              LPC2148 ARM7
                   |
       +-----------+-----------+
       |           |           |
      LCD         KPM         RTC
       |           |           |
   16×2 LCD     4×4 Keypad  Date & Time
                   |
             ATM Application
                   |
       +-----------+-----------+
       |           |           |
   Account      PIN         ATM Services
 Verification Verification      |
                              +-- Balance
                              +-- Withdraw
                              +-- Deposit
                              +-- Change PIN
                              +-- Mini Statement
                   |
                  UART
                   |
             Serial Terminal
                   |
          Transaction Logging
```

## 🔌 Hardware Interfacing

### 1. LPC2148 ARM7 Microcontroller

The LPC2148 acts as the central processing unit of the project.

Responsibilities:
- Executes ATM application logic.
- Controls peripheral communication.
- Processes keypad inputs.
- Updates LCD information.
- Maintains RTC operations.
- Handles external interrupts.
- Transmits transaction information through UART.

### 2. 16×2 LCD Interfacing

The LCD provides a visual interface between the user and the ATM system.

It displays:
- Welcome messages.
- Account number entry.
- PIN verification status.
- Available banking services.
- Transaction results.
- Balance information.
- Date and time information.

### 3. 4×4 Matrix Keypad Interfacing

The keypad allows users to interact with the ATM.

It is used for:
- Account number entry.
- PIN entry.
- Selecting ATM services.
- Entering deposit and withdrawal amounts.
- Modifying PIN information.
- Editing RTC date and time.

### 4. RTC Interfacing

The internal RTC peripheral of LPC2148 maintains the current date and time.

It is used to associate transaction records with their corresponding date and time.

### 5. UART Interfacing

UART provides serial communication between the microcontroller and a computer.

It is used to transmit transaction details for monitoring and debugging through a serial terminal.

## ✨ Features

- Account number verification.
- Four-digit PIN authentication.
- Three consecutive incorrect PIN attempts trigger a temporary account lock.
- Balance enquiry.
- Cash withdrawal.
- Cash deposit.
- PIN modification.
- Mini statement with the latest two transactions.
- RTC-based date and time management.
- External interrupt-based RTC editing.
- UART-based transaction logging.
- Modular peripheral driver implementation.
- Reusable application and service modules.

## 🏦 ATM Operations

### 1. Balance Enquiry

Displays the currently available account balance on the LCD.

### 2. Cash Withdrawal

Allows the user to withdraw an amount from the available balance.

The system checks whether sufficient balance is available before completing the operation.

### 3. Cash Deposit

Allows the user to deposit an amount and updates the available balance.

### 4. Change PIN

Allows an authenticated user to modify the existing PIN.

### 5. Mini Statement

Maintains and displays the latest two transaction records.

The transaction history includes relevant transaction information such as operation type and amount.

### 6. Exit

Terminates the current ATM session and returns the system to its initial state.

## 🔄 System Working Flow

```text
              System Initialization
                       |
                       v
                Initialize LCD
                       |
                       v
                Initialize KPM
                       |
                       v
                Initialize RTC
                       |
                       v
                Initialize UART
                       |
                       v
               Verify RTC Date/Time
                       |
                       v
              Enter Account Number
                       |
                       v
                Verify Account
                       |
                       v
                  Enter PIN
                       |
                       v
               Verify PIN
                       |
             +---------+---------+
             |                   |
          Valid PIN          Invalid PIN
             |                   |
             v                   v
         ATM Menu          Retry Attempts
             |                   |
             v             Three Failures?
       Select Service            |
             |                   v
             v             Account Restart
       Execute Service
             |
             v
      Update Transaction
             |
             v
       UART Log Output
             |
             v
        Return to Menu
```

## 🔐 PIN Authentication and Security

The project implements account number and PIN verification before allowing access to ATM services.

Security-related functionality includes:

- Account number verification.
- Four-digit PIN authentication.
- Maximum three consecutive incorrect PIN attempts.
- Temporary account blocking after three failed attempts.
- Restarting account verification without resetting the microcontroller.
- PIN verification before accessing protected operations.

## 🕒 RTC-Based Date and Time Management

The internal Real-Time Clock maintains date and time information.

RTC functionality includes:

- Reading the current date.
- Reading the current time.
- Displaying date and time information.
- Associating transaction records with timestamps.
- Editing date and time through an external interrupt mechanism.

An external interrupt is used to enter the RTC editing procedure.

## 📡 Real-Time Transaction Logging

UART communication is used to transmit transaction information from the microcontroller to a serial terminal.

Whenever a transaction takes place, relevant information is collected and transmitted through the UART interface.

Transaction information includes:

- Transaction type.
- Transaction amount.
- Transaction date.
- Transaction time.

**Example serial terminal output:**

```text
--------------------------------
       ATM TRANSACTION LOG
--------------------------------

Transaction : DEPOSIT
Amount      : 1000
Date        : 08/11/2026
Time        : 10:30:15

--------------------------------

Transaction : WITHDRAW
Amount      : 500
Date        : 08/11/2026
Time        : 10:35:20

--------------------------------
```

*Note: The above output is an illustrative example of the transaction log format.*

## ⚡ Interrupt Handling

External interrupt functionality is implemented for RTC date and time modification.

The interrupt mechanism allows the system to enter the RTC editing operation without continuously checking for a keypad input in the main application flow.

The interrupt service routine is designed to initiate the required operation while keeping interrupt processing as short as practical.

## 🧩 Software Architecture

The application is divided into multiple modules to improve readability, maintainability, and code reusability.

### Peripheral Driver Modules

- LCD driver
- Keypad driver
- Delay driver
- UART driver
- RTC driver

### Application Modules

- Main application
- ATM services
- PIN verification
- RTC editing
- UART transaction logging

Each module is responsible for a specific functionality, reducing dependency between different application components.

## 📂 Project Directory Structure

```text
ATM-Simulator-with-Real-Time-Transaction-Logging/
│
├── main.c
│
├── lcd.c
├── lcd.h
├── lcd_defines.h
│
├── kpm.c
├── kpm.h
│
├── delay.c
├── delay.h
│
├── rtc.c
├── rtc.h
│
├── rtc_edit.c
├── rtc_edit.h
│
├── uart.c
├── uart.h
│
├── uart_log.c
├── uart_log.h
│
├── atm.c
├── atm.h
│
├── pin_verification.c
├── pin_verification.h
│
└── README.md
```

*The directory structure represents the logical module organization. Adjust filenames to match the actual files uploaded to the repository.*

## 🧠 Embedded Concepts Implemented

| Concept | Implementation |
|---|---|
| Embedded C | Complete application development |
| ARM7 Architecture | LPC2148 microcontroller |
| GPIO | LCD and keypad interfacing |
| LCD Interfacing | Display operations |
| Matrix Keypad | User input |
| RTC | Date and time management |
| UART | Transaction logging |
| Interrupts | RTC editing |
| Modular Programming | Separate driver and application modules |
| Header Guards | Preventing multiple header inclusions |
| String Handling | Account and PIN processing |
| State Management | ATM operation flow |
| Data Management | Balance and transaction history |

## 🧪 Simulation and Testing

The project is developed and tested using embedded development and simulation tools.

Testing covers the following operations:

| Test Case | Expected Result |
|---|---|
| Valid account number | Account verification successful |
| Invalid account number | Access denied |
| Correct PIN | ATM menu displayed |
| Incorrect PIN | Retry message displayed |
| Three incorrect PIN attempts | Temporary account blocking |
| Balance enquiry | Available balance displayed |
| Valid withdrawal | Balance updated |
| Insufficient balance | Withdrawal rejected |
| Deposit | Balance updated |
| Change PIN | New PIN stored |
| Mini statement | Latest two transactions displayed |
| RTC modification | Date and time updated |
| UART logging | Transaction details transmitted |

## 🌍 Applications

The concepts implemented in this project can be extended to:

- Embedded banking terminals.
- Digital payment terminals.
- Microcontroller-based transaction systems.
- Access control systems.
- Embedded data logging applications.
- Real-time monitoring systems.

## ⚠️ Limitations

- This is an educational ATM simulation, not a production banking system.
- Account and PIN information are maintained in microcontroller memory.
- Transaction history is limited to the latest two records.
- UART logging is intended for monitoring and demonstration.
- No real cash dispenser or banking server integration is implemented.
- The temporary account blocking mechanism is a demonstration feature, not a secure banking-grade lockout system.

## 🚀 Future Improvements

- Integrate EEPROM or external Flash for persistent account storage.
- Implement multiple user accounts.
- Add an SD card for permanent transaction storage.
- Implement a real cash dispensing mechanism.
- Add a communication interface to a banking server.
- Implement encrypted authentication.
- Extend transaction history storage.
- Add a real-time transaction monitoring interface.

## 📚 Learning Outcomes

Through this project, I gained practical experience in:

- Developing embedded applications using Embedded C.
- Working with the ARM7 LPC2148 microcontroller.
- Interfacing LCD and matrix keypad peripherals.
- Understanding RTC configuration and operation.
- Implementing UART serial communication.
- Working with external interrupts.
- Designing reusable peripheral drivers.
- Organizing embedded applications into independent modules.
- Debugging and testing microcontroller-based applications.
- Integrating multiple peripherals into a single embedded application.

## 👨‍💻 Author

**Sai Charan Kancham**

Electronics and Communication Engineering

**Technical Interests:** Embedded Systems | Embedded C | ARM Microcontrollers | Firmware Development

- GitHub: https://github.com/SaiCharanKancham
- EW Skills: https://www.ewskills.com/user/Kanchamsaicharan

---

⭐ If you find this project useful, feel free to explore the repository and share your feedback.

**Project:** ATM Simulator with Real-Time Transaction Logging

**Platform:** ARM7 – LPC2148

**Programming Language:** Embedded C
