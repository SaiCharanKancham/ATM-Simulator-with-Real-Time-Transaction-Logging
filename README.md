# ATM Simulator with Real-Time Transaction Logging

An ARM7-based embedded ATM Simulator developed using the LPC2148 microcontroller and Embedded C. The project integrates multiple hardware peripherals to implement ATM operations, PIN authentication, RTC-based date and time management, and UART-based transaction logging.

The primary objective of this project is to understand how individual microcontroller peripherals work and how they can be integrated into a complete embedded application.

---

# 📑 Table of Contents

1. [Project Overview](#-project-overview)
2. [Project Objectives](#-project-objectives)
3. [Hardware Requirements](#-hardware-requirements)
4. [Software Requirements](#-software-requirements)
5. [System Architecture](#-system-architecture)
6. [Hardware Implementation](#-hardware-implementation)
7. [Peripheral Documentation](#-peripheral-documentation)
   - [16×2 LCD Interfacing](#1-16x2-lcd-interfacing)
   - [4×4 Matrix Keypad Interfacing](#2-4x4-matrix-keypad-interfacing)
   - [Real-Time Clock (RTC)](#3-real-time-clock-rtc)
   - [UART Communication](#4-uart-communication)
   - [External Interrupts](#5-external-interrupts)
8. [ATM Application Features](#-atm-application-features)
9. [PIN Authentication](#-pin-authentication)
10. [Transaction Logging](#-transaction-logging)
11. [UART-Based Log File Generation](#-uart-based-log-file-generation)
12. [Complete System Working Flow](#-complete-system-working-flow)
13. [Software Architecture](#-software-architecture)
14. [Project Directory Structure](#-project-directory-structure)
15. [Testing and Results](#-testing-and-results)
16. [Limitations](#-limitations)
17. [Future Improvements](#-future-improvements)
18. [Learning Outcomes](#-learning-outcomes)
19. [Author](#-author)

---

# 📖 Project Overview

The ATM Simulator is an embedded systems application developed using the LPC2148 ARM7 microcontroller.

The system provides an interactive interface through a 16×2 LCD and a 4×4 matrix keypad. Users can enter their account number and PIN to access different ATM services.

The application supports balance enquiry, cash withdrawal, cash deposit, PIN modification, and mini statement generation.

The internal Real-Time Clock maintains the current date and time, while UART communication is used to transmit transaction details to a computer through a serial interface.

The received UART data is monitored using Windows XP HyperTerminal and captured into a log file for transaction record maintenance.

The project follows a modular programming approach, where each peripheral has a separate driver module and the ATM application manages the overall system functionality.

# 🎯 Project Objectives

- Understand ARM7 LPC2148 microcontroller architecture.
- Develop reusable peripheral drivers using Embedded C.
- Interface LCD and matrix keypad with GPIO.
- Understand RTC configuration and operation.
- Implement UART serial communication.
- Configure and handle external interrupts.
- Develop PIN-based authentication.
- Implement basic ATM banking operations.
- Maintain transaction information with timestamps.
- Transfer transaction information to a computer using UART.
- Monitor serial communication using HyperTerminal.
- Capture and maintain transaction records in a log file.
- Integrate multiple peripherals into a single embedded application.

# 🔧 Hardware Requirements

| Component | Purpose |
|---|---|
| LPC2148 ARM7 | Main microcontroller |
| 16×2 LCD | Display interface |
| 4×4 Matrix Keypad | User input |
| Internal RTC | Date and time management |
| UART Interface | Serial transaction logging |
| External Interrupt Switch | Initiates RTC date and time editing |
| Computer | Serial monitoring and log file generation |
| USB-to-Serial Interface | Serial communication with computer |

# 💻 Software Requirements

| Software | Purpose |
|---|---|
| Keil µVision | Embedded C development |
| Proteus | Hardware simulation |
| Windows XP | Serial communication environment |
| HyperTerminal | UART monitoring and log file capture |

# 🏗️ System Architecture

The LPC2148 acts as the central controller and coordinates communication between different peripherals.

The LCD and matrix keypad provide the user interface, while the internal RTC maintains date and time information. UART is used to transmit transaction details to a computer for serial monitoring and logging.

![System Architecture](https://github.com/user-attachments/assets/bab64901-fa39-4856-a421-779ed764c83c)

# 🛠️ Hardware Implementation

The ATM Simulator was implemented using an LPC2148 ARM7 development board interfaced with different peripherals.

The following figure shows the physical hardware setup and peripheral connections used in the project.

<img width="1080" height="1350" alt="ATM" src="https://github.com/user-attachments/assets/32b48237-27d3-46a3-8267-86546450cda7" />



The hardware setup includes:

- LPC2148 ARM7 development board.
- 16×2 LCD for displaying ATM information.
- 4×4 matrix keypad for user input.
- UART interface for serial communication.
- External interrupt switch for RTC modification.
- USB power and communication connections.

# 🔌 Peripheral Documentation

This section explains the working principles, internal operation, interfacing techniques, and practical implementation of each peripheral used in the project.

Each peripheral is documented separately to make the hardware and software concepts easier to understand.

## 1. 16×2 LCD Interfacing

### Introduction

A 16×2 Liquid Crystal Display is an output device used to display information from a microcontroller.

It consists of two rows, with each row capable of displaying 16 characters.

In this project, the LCD provides the user interface for displaying ATM messages and operation results.

### Working Principle

The LCD receives commands and data from the microcontroller.

- Command mode configures LCD operations.
- Data mode displays characters.
- RS selects command or data mode.
- EN enables the LCD to accept the transferred information.
- Data pins carry the command or character information.

### LCD Interfacing Diagram

![16x2 LCD Pinout](https://github.com/user-attachments/assets/cf55dc34-04b4-4f3b-b7dc-c7559dd4f9c4)

### Important Signals

| Signal | Function |
|---|---|
| RS | Selects command or data register |
| RW | Selects read or write operation |
| EN | Enables LCD operation |
| D0–D7 | Data lines |
| VSS | Ground |
| VDD | Supply voltage |
| VEE | Contrast adjustment |

### Software Working Flow

```text
Initialize LCD
     |
Configure LCD Mode
     |
Send Initialization Commands
     |
Position Cursor
     |
Send Character Data
     |
Display Message
```

### Application in ATM Project

The LCD is used to:

- Display welcome messages.
- Request account number and PIN.
- Display ATM menu options.
- Show account balance.
- Display deposit and withdrawal results.
- Display transaction status.
- Show RTC date and time information.

---

## 2. 4×4 Matrix Keypad Interfacing

### Introduction

A 4×4 matrix keypad is an input device consisting of 16 push buttons arranged in four rows and four columns.

Each key is connected between one row and one column.

Instead of using 16 individual GPIO pins, the keypad requires only eight GPIO pins.

### Keypad Architecture

![4x4 Matrix Keypad Architecture](https://github.com/user-attachments/assets/0aec709b-de97-4eea-84c1-28268478e28e)

### Working Principle

The keypad operates using a row-column scanning technique.

1. Configure row pins as outputs.
2. Configure column pins as inputs.
3. Activate one row at a time.
4. Read the column inputs.
5. Identify the pressed key using its row and column positions.
6. Apply appropriate key debouncing.
7. Return the detected key value to the application.

### LPC2148 Interfacing

![LPC2148 Keypad Connections](https://github.com/user-attachments/assets/17d50e81-e64f-4d12-8eb9-c2ee173b1b3b)

### Application in ATM Project

The keypad is used for:

- Entering account numbers.
- Entering four-digit PINs.
- Selecting ATM menu options.
- Entering deposit and withdrawal amounts.
- Changing account PIN.
- Entering RTC date and time values.

---

## 3. Real-Time Clock (RTC)

### Introduction

A Real-Time Clock is a peripheral used to maintain current date and time information.

The LPC2148 contains an internal RTC peripheral that can maintain seconds, minutes, hours, days, months, and years.

### Working Principle

The RTC uses a clock source and prescaler configuration to maintain time.

It continuously updates the date and time registers.

The application reads these registers whenever the current date or time is required.

### RTC Working Flow

```text
Initialize RTC
     |
Configure Prescaler
     |
Enable RTC Clock
     |
Read Date and Time
     |
Convert Register Values
     |
Display on LCD
     |
Provide Timestamp to UART
```

### Application in ATM Project

The RTC is used to:

- Maintain current date and time.
- Display date and time information.
- Associate transaction records with timestamps.
- Support date and time modification.

---

## 4. UART Communication

### Introduction

UART stands for Universal Asynchronous Receiver Transmitter.

It is a serial communication mechanism used to transfer data between devices without a separate clock signal.

UART communication uses two primary signals:

- TX – Transmit data.
- RX – Receive data.

### Working Principle

UART transfers data in a serial format using a predefined baud rate.

A typical UART frame contains:

1. Start bit.
2. Data bits.
3. Optional parity bit.
4. Stop bit or bits.

Both communicating devices must use compatible communication settings.

### UART Interfacing Diagram

![UART Communication](https://github.com/user-attachments/assets/915818ca-806f-4a28-b6b8-927869b3069f)

### Important UART Registers

| Register | Purpose |
|---|---|
| U0THR | Transmit holding register |
| U0RBR | Receive buffer register |
| U0DLL | Baud rate divisor latch low |
| U0DLM | Baud rate divisor latch high |
| U0LCR | Line control register |
| U0LSR | Line status register |
| U0FCR | FIFO control register |
| U0IER | Interrupt enable register |

### UART Working Flow

```text
Initialize UART
     |
Configure PINSEL
     |
Set Baud Rate
     |
Configure Frame Format
     |
Check Transmit Status
     |
Transmit Character
     |
Complete Data Transmission
```

### Application in ATM Project

UART is used for real-time transaction monitoring.

Whenever an ATM transaction occurs, the application collects relevant information and sends it to the computer through the UART interface.

Transaction information includes:

- Transaction type.
- Transaction amount.
- Transaction date.
- Transaction time.

The received information is displayed using Windows XP HyperTerminal and can be captured into a text file.

---

## 5. External Interrupts

### Introduction

An interrupt is a mechanism that temporarily changes the normal program execution flow when a particular event occurs.

The LPC2148 supports external interrupt inputs that can be configured to respond to specific signal conditions.

### Working Principle

When a configured external interrupt event occurs:

1. The microcontroller detects the interrupt.
2. The interrupt controller identifies the interrupt source.
3. The processor branches to the corresponding interrupt service routine.
4. The ISR performs the required operation.
5. The processor returns to the previous execution flow.

### Important Registers

| Register | Purpose |
|---|---|
| PINSEL | Configures pin functionality |
| EXTINT | Clears external interrupt flags |
| EXTMODE | Selects level or edge triggering |
| EXTPOLAR | Selects interrupt polarity |
| VICIntEnable | Enables interrupt sources |
| VICVectAddr | Vector address configuration |

### Interrupt Working Flow

```text
Normal Program Execution
         |
   External Event
         |
   Interrupt Detection
         |
      Execute ISR
         |
 Perform Required Action
         |
  Clear Interrupt Flag
         |
  Return from ISR
```

### Application in ATM Project

An external interrupt is used to initiate RTC date and time editing.

This provides an interrupt-driven mechanism for entering the RTC modification procedure instead of continuously checking for the event in the normal application flow.

---

# 🏦 ATM Application Features

The ATM application supports the following operations.

| Operation | Description |
|---|---|
| Account Verification | Checks the entered account number |
| PIN Verification | Validates the four-digit PIN |
| Balance Enquiry | Displays available balance |
| Cash Withdrawal | Deducts the requested amount |
| Cash Deposit | Adds the deposited amount |
| Change PIN | Modifies the existing PIN |
| Mini Statement | Displays the latest two transactions |
| Exit | Ends the current ATM session |

# 🔐 PIN Authentication

The application implements account number and PIN verification.

Security features:

- Account number validation.
- Four-digit PIN verification.
- Three consecutive incorrect PIN attempts.
- Temporary account blocking after three failed attempts.
- Account verification restart without resetting the microcontroller.

# 📡 Transaction Logging

Transaction logging is implemented using UART communication.

Whenever an ATM transaction takes place, the application collects the relevant transaction information and transmits it to the computer through the UART interface.

The internal RTC supplies the timestamp associated with the transaction.

The transaction information includes:

- Transaction type.
- Transaction amount.
- Transaction date.
- Transaction time.

The latest two transactions are also maintained in the microcontroller memory for the mini statement feature.

# 📁 UART-Based Log File Generation

One of the important features of this project is maintaining transaction records on a computer using UART communication.

### Communication Setup

The LPC2148 development board is connected to the computer through a USB-to-Serial interface.

Windows XP HyperTerminal is used to establish serial communication and monitor the transaction information transmitted by the microcontroller.

### Working Procedure

1. Initialize UART communication in the LPC2148.
2. Configure the appropriate baud rate and serial frame format.
3. Connect the LPC2148 UART TX signal to the computer's serial interface through the appropriate USB-to-Serial interface.
4. Configure Windows XP HyperTerminal with matching serial communication settings.
5. Execute ATM operations such as balance enquiry, withdrawal, and deposit.
6. The microcontroller transmits transaction details through UART.
7. HyperTerminal receives and displays the transmitted information.
8. Use the HyperTerminal text capture feature to save the received serial data into a log file.

### Log File Generation Flow

```text
ATM Transaction
       |
Collect Transaction Details
       |
Read RTC Date and Time
       |
Format Transaction Information
       |
Transmit Through UART
       |
Computer Serial Interface
       |
Windows XP HyperTerminal
       |
Display Received Information
       |
Capture Text to File
       |
Transaction Log File
```

### Transaction Log File

The captured UART output is stored in a text file on the computer.

The log file provides a record of ATM transactions along with their corresponding date and time information.


### Advantages

- Provides transaction monitoring on a computer.
- Maintains transaction records outside the microcontroller.
- Demonstrates practical UART communication.
- Helps verify the transmitted transaction information.
- Provides a simple method for maintaining transaction history.

# 🔄 Complete System Working Flow

```text
       System Initialization
                |
       Initialize Peripherals
                |
       Verify RTC Date/Time
                |
       Enter Account Number
                |
       Account Verification
                |
           Enter PIN
                |
         PIN Verification
                |
          Display Menu
                |
         Select Operation
                |
       Execute ATM Service
                |
        Update Account Data
                |
       Record Transaction
                |
         UART Logging
                |
       Capture Log on PC
                |
        Return to Menu
```

# 🧩 Software Architecture

The project follows a modular programming approach.

Each peripheral has an independent driver module, while application-level modules handle ATM functionality.

### Peripheral Drivers

- LCD driver
- Keypad driver
- RTC driver
- UART driver
- Delay driver

### Application Modules

- Main application
- ATM services
- PIN verification
- RTC editing
- UART transaction logging

# 📂 Project Directory Structure

```text
ATM-Simulator/
│
├── header_files/
│   ├── atm.h
│   ├── defines.h
│   ├── delay.h
│   ├── kpm.h
│   ├── kpm_defines.h
│   ├── lcd.h
│   ├── lcd_defines.h
│   ├── rtc.h
│   ├── rtc_edit.h
│   ├── types.h
│   ├── uart.h
│   └── uart_log.h
│
├── source_files/
│   ├── atm_services.c
│   ├── delay_def.c
│   ├── kpm.c
│   ├── lcd.c
│   ├── main.c
│   ├── rtc.c
│   ├── rtc_edit.c
│   ├── uart.c
│   └── uart_log.c
│
├── videos/
│   ├── ATM.png
│   ├── ATM_Hardware_Demonstration.mp4
│   └── ATM_Transaction_Log_Demonstration.mp4
│
└── README.md
```
# 🧪 Testing and Results

The following test cases are used to validate the application.

| Test Case | Expected Result |
|---|---|
| Correct account number | Account accepted |
| Incorrect account number | Access rejected |
| Correct PIN | ATM menu displayed |
| Incorrect PIN | Retry message displayed |
| Three failed PIN attempts | Temporary blocking |
| Balance enquiry | Balance displayed |
| Valid withdrawal | Balance updated |
| Insufficient balance | Transaction rejected |
| Deposit | Balance updated |
| Change PIN | New PIN accepted |
| Mini statement | Latest two transactions displayed |
| RTC editing | Date/time modified |
| UART transmission | Transaction log received |
| HyperTerminal monitoring | Received data displayed |
| Log file capture | Serial data saved to a text file |

# ⚠️ Limitations

- This is an educational ATM simulation.
- Account details are maintained in microcontroller memory.
- Transaction history is limited to the latest two records.
- UART logging is intended for serial monitoring.
- Log file generation depends on the connected computer and serial terminal software.
- No actual cash dispenser or banking server is integrated.
- The authentication mechanism is not designed for production banking security.

# 🚀 Future Improvements

- Add EEPROM-based account storage.
- Support multiple user accounts.
- Add SD card-based transaction storage.
- Integrate an actual cash dispensing mechanism.
- Add encrypted authentication.
- Implement a graphical transaction monitoring interface.
- Extend transaction history storage.
- Implement automated transaction log management.

# 📚 Learning Outcomes

- ARM7 LPC2148 microcontroller programming.
- Embedded C application development.
- GPIO configuration and peripheral interfacing.
- LCD and matrix keypad driver development.
- RTC configuration and time management.
- UART serial communication.
- External interrupt configuration.
- Modular programming and reusable drivers.
- Hardware simulation and debugging.
- Serial communication between microcontroller and computer.
- HyperTerminal configuration and monitoring.
- Capturing UART data into text log files.
- Integration of multiple peripherals into one embedded application.

# 👨‍💻 Author

**Sai Charan Kancham**

Electronics and Communication Engineering

**Technical Interests:** Embedded Systems | Embedded C | ARM Microcontrollers | Firmware Development

- GitHub: https://github.com/SaiCharanKancham
- EW Skills: https://www.ewskills.com/user/Kanchamsaicharan

---

⭐ If you find this project useful, feel free to explore the source code and documentation.
