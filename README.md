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
6. [Peripheral Documentation](#-peripheral-documentation)
   - [16×2 LCD Interfacing](#1-16x2-lcd-interfacing)
   - [4×4 Matrix Keypad Interfacing](#2-4x4-matrix-keypad-interfacing)
   - [Real-Time Clock (RTC)](#3-real-time-clock-rtc)
   - [UART Communication](#4-uart-communication)
   - [External Interrupts](#5-external-interrupts)
7. [ATM Application Features](#-atm-application-features)
8. [PIN Authentication](#-pin-authentication)
9. [Transaction Logging](#-transaction-logging)
10. [Complete System Working Flow](#-complete-system-working-flow)
11. [Software Architecture](#-software-architecture)
12. [Project Directory Structure](#-project-directory-structure)
13. [Testing and Results](#-testing-and-results)
14. [Limitations](#-limitations)
15. [Future Improvements](#-future-improvements)
16. [Learning Outcomes](#-learning-outcomes)
17. [Author](#-author)

---

# 📖 Project Overview

The ATM Simulator is an embedded systems application developed using the LPC2148 ARM7 microcontroller.

The system provides an interactive interface through a 16×2 LCD and a 4×4 matrix keypad. Users can enter their account number and PIN to access different ATM services.

The application supports balance enquiry, cash withdrawal, cash deposit, PIN modification, and mini statement generation.

The internal Real-Time Clock maintains the current date and time, while UART communication is used to transmit transaction details to a serial terminal.

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
- Integrate multiple peripherals into a single embedded application.

# 🔧 Hardware Requirements

| Component | Purpose |
|---|---|
| LPC2148 ARM7 | Main microcontroller |
| 16×2 LCD | Display interface |
| 4×4 Matrix Keypad | User input |
| Internal RTC | Date and time management |
| UART | Serial transaction logging |
| Computer | Development and monitoring |

# 💻 Software Requirements

| Software | Purpose |
|---|---|
| Keil µVision | Embedded C development |
| Proteus | Hardware simulation |

# 🏗️ System Architecture

The LPC2148 acts as the central controller and coordinates communication between different peripherals.

```text
                  LPC2148 ARM7
                       |
       +---------------+----------------+
       |               |                |
      LCD            Keypad             RTC
       |               |                |
   16×2 Display    4×4 Matrix       Date & Time
       |               |                |
       +---------------+----------------+
                       |
                 ATM Application
                       |
       +---------------+----------------+
       |               |                |
   PIN Verification  ATM Services   RTC Editing
                       |
                 Transaction Data
                       |
                      UART
                       |
                Serial Terminal
                       |
               Transaction Logs
```
![Uploading image.png…]()


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

<img width="500" height="400" alt="16x2-Character-LCD-Display-Pinout" src="https://github.com/user-attachments/assets/cf55dc34-04b4-4f3b-b7dc-c7559dd4f9c4" />

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

**Driver files:** `lcd.c`, `lcd.h`, `lcd_defines.h`

---

## 2. 4×4 Matrix Keypad Interfacing

### Introduction

A 4×4 matrix keypad is an input device consisting of 16 push buttons arranged in four rows and four columns.

Each key is connected between one row and one column.

Instead of using 16 individual GPIO pins, the keypad requires only eight GPIO pins.

### Keypad Architecture

<img width="934" height="1038" alt="image" src="https://github.com/user-attachments/assets/0aec709b-de97-4eea-84c1-28268478e28e" />


### Working Principle

The keypad operates using a row-column scanning technique.

1. Configure row pins as outputs.
2. Configure column pins as inputs.
3. Activate one row at a time.
4. Read the column inputs.
5. Identify the pressed key using its row and column positions.
6. Apply appropriate key debouncing.
7. Return the detected key value to the application.

### Keypad Scanning Flowchart

```text
       Start
         |
   Configure GPIO
         |
   Activate First Row
         |
   Read Column Inputs
         |
   Key Detected?
      /     \
    No       Yes
    |         |
Next Row   Identify Key
    |         |
All Rows?  Debouncing
    |         |
    +------ Return Key
```

### LPC2148 Interfacing

<img width="1500" height="920" alt="lpc2148_keypad_all_8_gpio_connections" src="https://github.com/user-attachments/assets/17d50e81-e64f-4d12-8eb9-c2ee173b1b3b" />


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

It is a serial communication protocol used to transfer data between devices without a separate clock signal.

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

<img width="1024" height="572" alt="image" src="https://github.com/user-attachments/assets/915818ca-806f-4a28-b6b8-927869b3069f" />


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

Whenever an ATM transaction occurs, the application collects relevant information and sends it to the serial terminal.

Transaction information includes:

- Transaction type.
- Transaction amount.
- Transaction date.
- Transaction time.

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

The application collects transaction information and transmits it to the serial terminal.

The internal RTC supplies the timestamp associated with the transaction.

The latest two transactions are also maintained for the mini statement feature.

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
ATM-Simulator-with-Real-Time-Transaction-Logging/
│
├── README.md
│
├── Drivers/
│   ├── LCD/
│   ├── Keypad/
│   ├── RTC/
│   ├── UART/
│   └── Delay/
│
├── Application/
│   ├── main.c
│   ├── atm.c
│   ├── atm.h
│   ├── pin_verification.c
│   ├── pin_verification.h
│   ├── rtc_edit.c
│   ├── rtc_edit.h
│   ├── uart_log.c
│   └── uart_log.h
│
├── Documentation/
│   ├── LCD/
│   ├── Keypad/
│   ├── RTC/
│   ├── UART/
│   └── Interrupts/
│
├── Simulation/
│
└── Images/
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

# ⚠️ Limitations

- This is an educational ATM simulation.
- Account details are maintained in microcontroller memory.
- Transaction history is limited to the latest two records.
- UART logging is intended for serial monitoring.
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
- Integration of multiple peripherals into one embedded application.

# 👨‍💻 Author

**Sai Charan Kancham**

Electronics and Communication Engineering

**Technical Interests:** Embedded Systems | Embedded C | ARM Microcontrollers | Firmware Development

- GitHub: https://github.com/SaiCharanKancham
- EW Skills: https://www.ewskills.com/user/Kanchamsaicharan

---

⭐ If you find this project useful, feel free to explore the source code and documentation.
