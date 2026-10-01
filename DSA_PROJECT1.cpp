/*
DSA PROJECT 1 - ATM SIMULATION

Programmed by:
Del Rosario, Bianca E.
Manalo, Daniella C.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <conio.h>
#include <cstdlib>

using namespace std;


// Sets the flash drive location.
string cardDrive = "D:\\";


// Stores the information of each account.
struct Node
{
    int accountNo;
    string accountName;
    string birthday;
    string contactNo;
    double balance;
    string pin;

    Node *next; 
};


Node *top = NULL;


void clearScreen()
{
    system("cls");
}

// Pauses the program.
void pauseScreen()
{
    cout << "\nPress ENTER to continue...";
    cin.get();
}

// Encrypts the PIN before saving it.
string encryptPIN(string pin)
{
    string encrypted = "";
    for(int i = 0; i < pin.length(); i++)
    {
        int digit = pin[i] - '0';
        digit = (digit + 3) % 10;
        encrypted += char(digit + '0');
    }
    return encrypted;
}

// Gets the PIN while hiding the digits.
string inputPIN()
{
    string pin;
    char ch;

    cout << "Enter PIN (4-6 digits): ";

    while(true)
    {
        ch = _getch();

        if(ch == 13)
        {
            if(pin.length() >= 4 && pin.length() <= 6)
            {
                cout << endl;
                return pin;
            }

            cout << "\nPIN must be 4 to 6 digits.\n";
            cout << "Enter PIN (4-6 digits): ";

            pin = "";
            continue;
        }

        // Removes the last PIN digit.
        if(ch == 8)
        {
            if(pin.length() > 0)
            {
                pin.pop_back();
                cout << "\b \b";
            }

            continue;
        }

        // Accepts digits only.
        if(ch >= '0' && ch <= '9')
        {
            if(pin.length() < 6)
            {
                pin += ch;
                cout << "*";
            }
        }
    }
}

// Searches for an account in the Linked List.
Node* searchAccount(int accountNo)
{
    Node *p = top;

    while(p != NULL)
    {
        if(p->accountNo == accountNo)
        {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

// Adds a new account to the Linked List.
void insertAccount(Node *newNode)
{
    newNode->next = NULL;

    // Checks if the List is empty.
    if(top == NULL)
    {
        top = newNode;
    }

    // Adds the node at the end of the List.
    else
    {
        Node *p = top;
        while(p->next != NULL)
        {
            p = p->next;
        }
        p->next = newNode;
    }
}

// Displays all accounts in the Linked List.
void displayAccounts()
{
    clearScreen();

    cout << "====================================================\n";
    cout << "                 ACCOUNT LIST\n";
    cout << "====================================================\n";

    if(top == NULL)
    {
        cout << "\nNo registered accounts.\n";
        pauseScreen();
        return;
    }

    Node *p = top;

    // Traverses the Linked List.
    while(p != NULL)
    {
        cout << "\nAccount Number : " << p->accountNo << endl;
        cout << "Account Name   : " << p->accountName << endl;
        cout << "Birthday       : " << p->birthday << endl;
        cout << "Contact Number : " << p->contactNo << endl;
        cout << fixed << setprecision(2);
        cout << "Balance        : P" << p->balance << endl;
        cout << "Encrypted PIN  : " << p->pin << endl;
        cout << "----------------------------------------------------\n";
        p = p->next;
    }
    pauseScreen();
}

// Deletes an account from the Linked List.
void deleteAccount(int accountNo)
{
    Node *p = top;
    Node *prev = NULL;

    // Checks if the List is empty.
    if(top == NULL)
    {
        return;
    }

    // Deletes the first node.
    if(top->accountNo == accountNo)
    {
        p = top;

        top = top->next;

        delete p;

        return;
    }

    // Searches for the account to delete.
    while(p != NULL)
    {
        if(p->accountNo == accountNo)
        {
            prev->next = p->next;
            delete p;
            return;
        }
        prev = p;
        p = p->next;
    }
}

// Generates a unique 5-digit account number.
int generateAccountNumber()
{
    int accountNo = 10000;
    while(searchAccount(accountNo) != NULL)
    {
        accountNo++;
        if(accountNo > 99999)
        {
            return -1;
        }
    }

    return accountNo;
}

// Saves all accounts to a text file.
void saveAccounts()
{
    ofstream file("accounts.txt");

    Node *p = top;

    // Writes each account to the file.
    while(p != NULL)
    {
        file << p->accountNo << endl;
        file << p->accountName << endl;
        file << p->birthday << endl;
        file << p->contactNo << endl;
        file << p->balance << endl;
        file << p->pin << endl;
        p = p->next;
    }
    file.close();
}

// Loads saved accounts into the Linked List.
void loadAccounts()
{
    ifstream file("accounts.txt");
    
    if(!file)
    {
        return;
    }

    while(true)
    {
        Node *newNode = new Node;

        if(!(file >> newNode->accountNo))
        {
            delete newNode;
            break;
        }

        file.ignore();
        getline(file, newNode->accountName);
        getline(file, newNode->birthday);
        getline(file, newNode->contactNo);

        // Reads the account balance.
        file >> newNode->balance;

        file.ignore();

        // Reads the encrypted PIN.
        getline(file, newNode->pin);

        newNode->next = NULL;
        insertAccount(newNode);
    }
    file.close();
}

// Checks if the ATM card contains pin.code.
bool checkCard()
{
    string fileName = cardDrive + "pin.code";
    ifstream card(fileName);

    if(card)
    {
        card.close();
        return true;
    }
    return false;
}

// Saves the account number and PIN to the card.
bool writeCard(int accountNo, string encryptedPIN)
{
    string fileName = cardDrive + "pin.code";
    ofstream card(fileName);

    // Checks if the card file can be opened.
    if(!card)
    {
        return false;
    }

    card << "ACCOUNT=" << accountNo << endl;
    card << "PIN=" << encryptedPIN << endl;
    card.close();
    return true;
}

// Reads the account number from the card.
int readCardAccount()
{
    string fileName = cardDrive + "pin.code";
    ifstream card(fileName);

    if(!card)
    {
        return -1;
    }

    string line;

    while(getline(card, line))
    {
        if(line.find("ACCOUNT=") == 0)
        {
            string number = line.substr(8);
            card.close();
            return stoi(number);
        }
    }
    card.close();
    return -1;
}

// Reads the encrypted PIN from the card.
string readCardPIN()
{
    string fileName = cardDrive + "pin.code";
    ifstream card(fileName);

    if(!card)
    {
        return "";
    }
    string line;

    while(getline(card, line))
    {
        if(line.find("PIN=") == 0)
        {
            string pin = line.substr(4);
            card.close();
            return pin;
        }
    }
    card.close();
    return "";
}

// Handles new ATM account registration.
void registration()
{
    clearScreen();

    cout << "====================================================\n";
    cout << "                REGISTRATION MODULE\n";
    cout << "====================================================\n\n";

    // Checks if the flash drive is inserted.
    cout << "Insert ATM Card / Flash Drive.\n";
    cout << "Card Location: " << cardDrive << endl;
    cout << "\nChecking card...\n";

    string fileName = cardDrive + "pin.code";
    ofstream card(fileName, ios::app);

    if(!card)
    {
        cout << "\nPlease insert card.\n";
        pauseScreen();
        return;
    }
    card.close();
    cout << "Card detected.\n";

    int accountNo = generateAccountNumber();

    if(accountNo == -1)
    {
        cout << "\nUnable to generate account number.\n";
        pauseScreen();
        return;
    }

    cout << "\nAssigned Account Number: "
        << accountNo << endl;

    string accountName;

    cout << "\nEnter Account Name: ";
    getline(cin, accountName);

    while(accountName == "")
    {
        cout << "Account name cannot be empty.\n";
        cout << "Enter Account Name: ";
        getline(cin, accountName);
    }

    string birthday;

    cout << "Enter Birthday: ";
    getline(cin, birthday);

    string contactNo;

    cout << "Enter Contact Number: ";
    getline(cin, contactNo);

    double initialDeposit;

    do
    {
        cout << "Enter Initial Deposit (Minimum P5000): P";
        cin >> initialDeposit;

        if(initialDeposit < 5000)
        {
            cout << "\nInitial deposit must be at least P5000.\n";
        }
    }while(initialDeposit < 5000);

    cin.ignore();

    cout << "\nCreate ATM PIN.\n";

    string pin = inputPIN();

    // Encrypts the PIN.
    string encryptedPIN = encryptPIN(pin);

    Node *newNode = new Node;

    newNode->accountNo = accountNo;
    newNode->accountName = accountName;
    newNode->birthday = birthday;
    newNode->contactNo = contactNo;
    newNode->balance = initialDeposit;
    newNode->pin = encryptedPIN;

    insertAccount(newNode);
    saveAccounts();

    // Saves the PIN to the flash drive.
    if(!writeCard(accountNo, encryptedPIN))
    {
        cout << "\nUnable to save PIN to ATM card.\n";
        pauseScreen();
        return;
    }
    clearScreen();

    cout << "====================================================\n";
    cout << "          REGISTRATION SUCCESSFUL\n";
    cout << "====================================================\n\n";

    cout << "Account Number : "
         << accountNo << endl;

    cout << "Account Name   : "
         << accountName << endl;

    cout << "Birthday       : "
         << birthday << endl;

    cout << "Contact Number : "
         << contactNo << endl;

    cout << fixed << setprecision(2);

    cout << "Initial Deposit: P"
         << initialDeposit << endl;

    cout << "\nPIN encrypted successfully.\n";
    cout << "PIN saved to ATM card.\n";
    cout << "Account inserted into Linked List.\n";
    pauseScreen();
}

// Checks the card and PIN before transactions.
Node* authenticate()
{
    clearScreen();

    cout << "====================================================\n";
    cout << "                  ATM LOGIN\n";
    cout << "====================================================\n\n";

    cout << "Insert ATM Card / Flash Drive.\n";
    cout << "Card Location: " << cardDrive << endl;

    // Checks if the card is inserted.
    if(!checkCard())
    {
        cout << "\nPlease insert card.\n";
        pauseScreen();
        return NULL;
    }

    cout << "\nCard detected.\n";

    int accountNo = readCardAccount();

    if(accountNo == -1)
    {
        cout << "\nInvalid card.\n";
        pauseScreen();
        return NULL;
    }

    Node *account = searchAccount(accountNo);

    if(account == NULL)
    {
        cout << "\nAccount is not enrolled.\n";
        pauseScreen();
        return NULL;
    }

    string cardPIN = readCardPIN();

    if(cardPIN == "")
    {
        cout << "\nPIN code not found on card.\n";
        pauseScreen();
        return NULL;
    }

    // Gets the user's PIN.
    cout << "\nAccount Number: "
         << accountNo << endl;

    string enteredPIN = inputPIN();
    string encryptedPIN = encryptPIN(enteredPIN);

    if(encryptedPIN != cardPIN)
    {
        cout << "\nIncorrect PIN.\n";
        pauseScreen();
        return NULL;
    }

    if(encryptedPIN != account->pin)
    {
        cout << "\nPIN does not match account record.\n";
        pauseScreen();
        return NULL;
    }

    cout << "\nPIN verified successfully.\n";
    cout << "Welcome, "
         << account->accountName
         << "!\n";

    pauseScreen();
    return account;
}

// Shows the current account balance.
void balanceInquiry(Node *account)
{
    clearScreen();

    cout << "====================================================\n";
    cout << "                 BALANCE INQUIRY\n";
    cout << "====================================================\n\n";

    cout << "Account Number : "
         << account->accountNo << endl;

    cout << "Account Name   : "
         << account->accountName << endl;

    cout << fixed << setprecision(2);

    cout << "Current Balance: P"
         << account->balance << endl;

    pauseScreen();
}

// Withdraws money from the account.
void withdraw(Node *account)
{
    clearScreen();

    cout << "====================================================\n";
    cout << "                    WITHDRAW\n";
    cout << "====================================================\n\n";

    cout << fixed << setprecision(2);

    cout << "Current Balance: P"
         << account->balance << endl;

    double amount;

    cout << "\nEnter amount to withdraw: P";
    cin >> amount;
    cin.ignore();

    if(amount <= 0)
    {
        cout << "\nInvalid amount.\n";
        pauseScreen();
        return;
    }

    if(amount > account->balance)
    {
        cout << "\nInsufficient balance.\n";
        pauseScreen();
        return;
    }

    // Deducts the withdrawal amount.
    account->balance -= amount;


    // Saves the updated balance.
    saveAccounts();

    cout << "\nWithdrawal successful.\n";

    cout << "Amount Withdrawn : P"
         << amount << endl;

    cout << "Remaining Balance: P"
         << account->balance << endl;

    pauseScreen();
}

// Adds money to the account.
void deposit(Node *account)
{
    clearScreen();

    cout << "====================================================\n";
    cout << "                     DEPOSIT\n";
    cout << "====================================================\n\n";

    cout << fixed << setprecision(2);

    cout << "Current Balance: P"
         << account->balance << endl;

    double amount;

    cout << "\nEnter amount to deposit: P";
    cin >> amount;
    cin.ignore();

    if(amount <= 0)
    {
        cout << "\nInvalid amount.\n";
        pauseScreen();
        return;
    }

    account->balance += amount;
    saveAccounts();

    cout << "\nDeposit successful.\n";

    cout << "Amount Deposited: P"
         << amount << endl;

    cout << "New Balance     : P"
         << account->balance << endl;

    pauseScreen();
}

// Transfers money to another enrolled account.
void fundTransfer(Node *sender)
{
    clearScreen();

    cout << "====================================================\n";
    cout << "                  FUND TRANSFER\n";
    cout << "====================================================\n\n";

    cout << "Your Account Number: "
         << sender->accountNo << endl;

    cout << fixed << setprecision(2);

    cout << "Your Balance: P"
         << sender->balance << endl;

    int recipientNo;

    cout << "\nEnter Recipient Account Number: ";
    cin >> recipientNo;
    cin.ignore();

    if(recipientNo == sender->accountNo)
    {
        cout << "\nYou cannot transfer to your own account.\n";
        pauseScreen();
        return;
    }

    Node *recipient =
        searchAccount(recipientNo);

    // Checks if the recipient is enrolled.
    if(recipient == NULL)
    {
        cout << "\nRecipient account not found.\n";
        cout << "Transfer is only available for enrolled accounts.\n";
        pauseScreen();
        return;
    }

    cout << "\nRecipient Name: "
         << recipient->accountName << endl;

    double amount;

    cout << "Enter Transfer Amount: P";
    cin >> amount;
    cin.ignore();

    if(amount <= 0)
    {
        cout << "\nInvalid amount.\n";
        pauseScreen();
        return;
    }

    if(amount > sender->balance)
    {
        cout << "\nInsufficient balance.\n";
        pauseScreen();
        return;
    }

    sender->balance -= amount;
    recipient->balance += amount;

    saveAccounts();

    cout << "\n====================================================\n";
    cout << "             TRANSFER SUCCESSFUL\n";
    cout << "====================================================\n\n";

    cout << "Sender   : "
         << sender->accountName << endl;

    cout << "Receiver : "
         << recipient->accountName << endl;

    cout << "Amount   : P"
         << amount << endl;

    cout << "New Balance: P"
         << sender->balance << endl;

    pauseScreen();
}

// Changes the account PIN.
void changePIN(Node *account)
{
    clearScreen();

    cout << "====================================================\n";
    cout << "                    CHANGE PIN\n";
    cout << "====================================================\n\n";

    // Checks if the card is inserted.
    if(!checkCard())
    {
        cout << "Please insert card.\n";
        pauseScreen();
        return;
    }

    cout << "Enter Current PIN.\n";

    string currentPIN = inputPIN();
    string encryptedCurrentPIN =
        encryptPIN(currentPIN);


    if(encryptedCurrentPIN != account->pin)
    {
        cout << "\nIncorrect current PIN.\n";
        pauseScreen();
        return;
    }


    cout << "\nEnter New PIN.\n";
    string newPIN = inputPIN();

    if(newPIN == currentPIN)
    {
        cout << "\nNew PIN cannot be the same as old PIN.\n";
        pauseScreen();
        return;
    }

    cout << "\nConfirm New PIN.\n";
    string confirmPIN = inputPIN();

    if(newPIN != confirmPIN)
    {
        cout << "\nPIN confirmation does not match.\n";
        pauseScreen();
        return;
    }

    string encryptedNewPIN =
        encryptPIN(newPIN);

    account->pin = encryptedNewPIN;

    // Updates the PIN on the flash drive.
    if(!writeCard(account->accountNo, encryptedNewPIN))
    {
        cout << "\nUnable to update card.\n";
        pauseScreen();
        return;
    }

   saveAccounts();

    cout << "\nPIN changed successfully.\n";
    cout << "New PIN saved to ATM card.\n";
    pauseScreen();
}

// Shows the ATM transaction options.
void transactionModule()
{
    Node *account = authenticate();

    if(account == NULL)
    {
        return;
    }

    int choice;

    do
    {
        clearScreen();

        cout << "====================================================\n";
        cout << "                ATM TRANSACTION MENU\n";
        cout << "====================================================\n\n";

        cout << "Account Name   : "
             << account->accountName << endl;

        cout << "Account Number : "
             << account->accountNo << endl;

        cout << "\n";

        cout << "1. Balance Inquiry\n";
        cout << "2. Withdraw\n";
        cout << "3. Deposit\n";
        cout << "4. Fund Transfer\n";
        cout << "5. Change PIN\n";
        cout << "6. Exit\n";

        cout << "\nEnter Choice: ";
        cin >> choice;
        cin.ignore();

        // Runs the selected transaction.
        switch(choice)
        {
            case 1:
                balanceInquiry(account);
                break;
            case 2:
                withdraw(account);
                break;
            case 3:
                deposit(account);
                break;
            case 4:
                fundTransfer(account);
                break;
            case 5:
                changePIN(account);
                break;
            case 6:
                cout << "\nThank you for using the ATM.\n";
                break;
            default:
                cout << "\nInvalid choice.\n";
                pauseScreen();
        }

    }while(choice != 6);
}

// Shows the main ATM menu.
void mainMenu()
{
    int choice;

    do
    {
        clearScreen();

        cout << "====================================================\n";
        cout << "                  ATM SIMULATION\n";
        cout << "====================================================\n\n";

        cout << "1. Registration\n";
        cout << "2. Transaction\n";
        cout << "3. Display Accounts\n";
        cout << "4. Exit\n";
        cout << "\nEnter Choice: ";
        cin >> choice;
        cin.ignore();

        switch(choice)
        {
            case 1:
                registration();
                break;
            case 2:
                transactionModule();
                break;
            case 3:
                displayAccounts();
                break;
            case 4:
                cout << "\nThank you for using the ATM Simulation.\n";
                break;
            default:
                cout << "\nInvalid choice.\n";
                pauseScreen();
        }

    }while(choice != 4);
}

// Starts the ATM program.
int main()
{
    loadAccounts();
    mainMenu();
    return 0;
}