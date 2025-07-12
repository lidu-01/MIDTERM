#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <algorithm>

using namespace std;

class Account {
private:
    string accountHolder;
    int accountNumber;
    double balance;
    string* transactionHistory; // for pionter to dynamic array for transactions
    int transactionCount;
    int maxTransactions;

public:
    // constructor 
    Account(string holder = "Unknown", int accNum = 0, double bal = 0.0) {
        accountHolder = holder;
        accountNumber = accNum;
        balance = bal;
        maxTransactions = 10; // Fixed-size array for transactions
        transactionHistory = new string
        [maxTransactions]; // dynamic allocation
        transactionCount = 0;
    }
void addTransaction(string transaction) {
    if (transactionCount < maxTransactions) {
        transactionHistory[transactionCount] = transaction;
            transactionCount++;
        } else {
            cout << "Transaction history full!" << endl;
    }
}
// Getters
    string getHolder() const { return accountHolder; }
    int getAccountNumber() const { return accountNumber; }
    double getBalance() const { return balance; }

    // Deposit money
    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            string trans = "Deposited $" + to_string(amount) + ", New Balance: $" + to_string(balance);
            addTransaction(trans);
        }
    }

    // Withdraw money
    bool withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            string trans = "Withdrew $" + to_string(amount) + ", New Balance: $" + to_string(balance);
            addTransaction(trans);
            return true;
        }
        return false;
    }

    // Display transaction history
    void displayHistory(ofstream& outFile) const {
        outFile << "\nTransaction History for Account " << accountNumber << " (" << accountHolder << "):\n";
        for (int i = 0; i < transactionCount; i++) {
            outFile << transactionHistory[i] << endl;
        }
    }
};

// Class 2: ATM to manage multiple accounts and operations
class ATM {
private:
    Account* accounts; // Pointer to dynamic array of accounts
    int accountCount;
    int maxAccounts;

public:
    // Constructor
    ATM(int max = 100) {
        maxAccounts = max;
        accounts = new Account[maxAccounts]; // Dynamic allocation
        accountCount = 0;
    }

    // Destructor
    ~ATM() {
        delete[] accounts;
    }

    // Add a new account
    void addAccount(string holder, int accNum, double initialBalance) {
        if (accountCount < maxAccounts) {
            accounts[accountCount] = Account(holder, accNum, initialBalance);
            accountCount++;
        } else {
            cout << "Cannot add more accounts!" << endl;
        }
    }

    // Binary search to find account by account number
    int binarySearch(int accNum) {
        if (accountCount == 0) {
            return -1; // No accounts to search
        }


        int left = 0, right = accountCount - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (accounts[mid].getAccountNumber() == accNum) {
                return mid;
            }
            if (accounts[mid].getAccountNumber() < accNum) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        return -1; // Account not found
    }

    // Perform deposit
    void deposit(int accNum, double amount, ofstream& outFile) {
        int index = binarySearch(accNum);
        if (index != -1) {
            accounts[index].deposit(amount);
            outFile << "Deposited $" << amount << " to Account " << accNum << endl;
        } else {
            outFile << "Account " << accNum << " not found!" << endl;
        }
    }

    // Perform withdrawal
    void withdraw(int accNum, double amount, ofstream& outFile) {
        int index = binarySearch(accNum);
        if (index != -1) {
            if (accounts[index].withdraw(amount)) {
                outFile << "Withdrew $" << amount << " from Account " << accNum << endl;
            } else {
                outFile << "Insufficient funds or invalid amount for Account " << accNum << endl;
            }
        } else {
            outFile << "Account " << accNum << " not found!" << endl;
        }
    }

    // Display account details
    void displayAccount(int accNum, ofstream& outFile) {
        int index = binarySearch(accNum);
        if (index != -1) {
            outFile << "\nAccount Details:\n";
            outFile << "Holder: " << accounts[index].getHolder() << endl;
            outFile << "Account Number: " << accounts[index].getAccountNumber() << endl;
            outFile << "Balance: $" << fixed << setprecision(2) << accounts[index].getBalance() << endl;
            accounts[index].displayHistory(outFile);
        } else {
            outFile << "Account " << accNum << " not found!" << endl;
        }
    }

    // Save all accounts to file
    void saveToFile(string filename) {
        ofstream outFile(filename, ios::app);
        if (outFile.is_open()) {
            for (int i = 0; i < accountCount; i++) {
                outFile << "Account: " << accounts[i].getAccountNumber() << ", Holder: "
                        << accounts[i].getHolder() << ", Balance: $" << fixed << setprecision(2)
                        << accounts[i].getBalance() << endl;
                accounts[i].displayHistory(outFile);
            }
            outFile.close();
        } else {
            cout << "Error opening file for writing!" << endl;
        }
    }
};

// Main function to demonstrate ATM functionality
int main() {
    // Initialize output file
    ofstream outFile("atm_output.txt");
    if (!outFile.is_open()) {
        cout << "Error opening output file!" << endl;
        return 1;
    }

    // Demonstrate data types and sizes
    outFile << "Data Type Sizes (in bytes):\n";
    outFile << "int: " << sizeof(int) << endl;
    outFile << "double: " << sizeof(double) << endl;
    outFile << "string: " << sizeof(string) << endl;
    outFile << "Account*: " << sizeof(Account*) << endl;

    // Create ATM instance
    ATM atm;

    // Add sample accounts
    atm.addAccount("John Doe", 1001, 500.0);
    atm.addAccount("Jane Smith", 1002, 1000.0);
    atm.addAccount("Alice Johnson", 1003, 250.0);

    // Perform transactions
    atm.deposit(1001, 200.0, outFile);
    atm.withdraw(1002, 300.0, outFile);
    atm.deposit(1003, 50.0, outFile);
    atm.withdraw(1001, 100.0, outFile);

    // Display account details
    atm.displayAccount(1001, outFile);
    atm.displayAccount(1002, outFile);
    atm.displayAccount(1004, outFile); // Non-existent account

    // Save to file
    atm.saveToFile("accounts.txt");

    outFile.close();
    cout << "ATM operations completed. Check atm_output.txt and accounts.txt for results." << endl;

    return 0;
}