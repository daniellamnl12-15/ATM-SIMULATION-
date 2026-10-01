# ATM Simulation

A simple ATM Simulation developed in C++ for **DSA Project 1**.  
This project demonstrates the use of a **pointer-based Linked List implementation of ADT List** in an ATM system.

## Features

### Registration Module
- Register a new ATM account
- Generate a unique 5-digit account number
- Store account information using a Linked List
- Set a 4–6 digit PIN
- Encrypt the PIN using a simple digit-shifting method
- Save the account number and encrypted PIN to a flash drive as `pin.code`
- Save account records to `accounts.txt`

### Transaction Module
- ATM card/PIN authentication
- Balance Inquiry
- Withdraw
- Deposit
- Fund Transfer between enrolled accounts
- Change PIN

### Other Features
- Display all registered accounts
- Search accounts using the Linked List
- Load saved accounts when the program starts
- Save updated account information to a text file

## Data Structure

The project uses a **pointer-based Linked List**.

Each `Node` represents one ATM account and contains:

- Account Number
- Account Name
- Birthday
- Contact Number
- Balance
- Encrypted PIN
- Pointer to the next account

Main Linked List operations used:

- Insert
- Search
- Traverse
- Delete

## ATM Card

A **flash drive is used as the ATM card**.

The program stores the account number and encrypted PIN in a file named:

```text
pin.code
