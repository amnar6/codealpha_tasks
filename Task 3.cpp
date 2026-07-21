/*
CodeAlpha Internship - Task 4
Banking System (medium version)

    Classes:
    - Transaction : one record of money movement, with a timestamp
    - Account : holds balance + transaction history for one customer
    - Customer : name, unique ID, and an Account (composition)
    - Bank : manages all customers; create, deposit, withdraw, transfer, view history, save/load to a file
*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
using namespace std;

const string DATA_FILE = "bank_data.txt";

//returns the current date/time as a short readable string
string currentTimestamp() 
{
    time_t now = time(0);
    tm* ltm = localtime(&now);
    stringstream ss;
    ss << (1900 + ltm->tm_year) << "-"
       << (1 + ltm->tm_mon) << "-"
       << ltm->tm_mday << " "
       << ltm->tm_hour << ":" << ltm->tm_min;

    return ss.str();
}

// Transaction
class Transaction {
public:
    string type;      // "Deposit", "Withdraw", "Transfer Out", "Transfer In"
    double amount;
    string timestamp;

    Transaction(string t, double a, string time) 
    {
        type = t;
        amount = a;
        timestamp = time;
    }

    //turns the transaction into one line of text, for saving to file
    string toFileLine() const {
        return type + "|" + to_string(amount) + "|" + timestamp;
    }
};


// Account
class Account {
private:
    int accountNumber;
    double balance;
    vector<Transaction> history;

public:
    Account(int accNum = 0, double startingBalance = 0.0) 
    {
        accountNumber = accNum;
        balance = startingBalance;
    }

    int getAccountNumber() const { return accountNumber; }
    double getBalance() const { return balance; }
    const vector<Transaction>& getHistory() const { return history; }

    bool deposit(double amount, string label = "Deposit") 
    {
        if (amount <= 0) {
            cout << "Invalid deposit amount." << endl;
            return false;
        }
        balance += amount;
        history.push_back(Transaction(label, amount, currentTimestamp()));
        return true;
    }

    bool withdraw(double amount, string label = "Withdraw") {
        if (amount <= 0) {
            cout << "Invalid withdrawal amount." << endl;
            return false;
        }
        if (amount > balance) {
            cout << "Insufficient balance." << endl;
            return false;
        }
        balance -= amount;
        history.push_back(Transaction(label, amount, currentTimestamp()));
        return true;
    }

    void showHistory() const {
        if (history.empty()) {
            cout << "  No transactions yet." << endl;
            return;
        }
        for (size_t i = 0; i < history.size(); i++) {
            cout << "  " << (i + 1) << ". [" << history[i].timestamp << "] "
                 << history[i].type << " - $" << history[i].amount << endl;
        }
    }
};

// Customer
class Customer {
public:
    int id;
    string name;
    Account account;

    Customer(int customerId, string customerName, int accNum, double startingBalance): account(accNum, startingBalance) {
        id = customerId;
        name = customerName;
    }

    void display() const {
        cout << "----------------------------------------" << endl;
        cout << "Customer ID : " << id << endl;
        cout << "Name : " << name << endl;
        cout << "Account Number : " << account.getAccountNumber() << endl;
        cout << "Balance : $" << account.getBalance() << endl;
        cout << "Transaction History: " << endl;
        account.showHistory();
        cout << "----------------------------------------" << endl;
    }
};

// Bank  (manages every customer + file persistence)
class Bank {
private:
    vector<Customer> customers;
    int nextCustomerId;
    int nextAccountNumber;

    Customer* findCustomer(int id) {
        for (auto& c : customers) {
            if (c.id == id) 
                return &c;
        }
        return nullptr;
    }

public:
    Bank() 
    {
        nextCustomerId = 1;
        nextAccountNumber = 1001; // just a starting point for account numbers
    }

    void addCustomer(string name, double startingBalance) 
    {
        Customer newCustomer(nextCustomerId, name, nextAccountNumber, startingBalance);
        customers.push_back(newCustomer);
        cout << "Customer added! ID: " << nextCustomerId << " | Account Number: " << nextAccountNumber << endl;
        nextCustomerId++;
        nextAccountNumber++;
    }

    void deposit(int id, double amount) {
        Customer* c = findCustomer(id);
        if (!c) { cout << "Customer not found." << endl; return; }
        if (c->account.deposit(amount)) {
            cout << "Deposited $" << amount << ". New balance: $" << c->account.getBalance() << endl;
        }
    }

    void withdraw(int id, double amount) {
        Customer* c = findCustomer(id);
        if (!c) { cout << "Customer not found." << endl; return; }
        if (c->account.withdraw(amount)) {
            cout << "Withdrew $" << amount << ". New balance: $" << c->account.getBalance() << endl;
        }
    }

    void transfer(int fromId, int toId, double amount) {
        Customer* from = findCustomer(fromId);
        Customer* to = findCustomer(toId);
        if (!from || !to) { cout << "One or both customer IDs are invalid." << endl; return; }
        if (fromId == toId) { cout << "Cannot transfer to the same account." << endl; return; }

        if (from->account.withdraw(amount, "Transfer Out")) {
            to->account.deposit(amount, "Transfer In");
            cout << "Transferred $" << amount << " from " << from->name << " to " << to->name << endl;
        }
    }

    void showCustomer(int id) {
        Customer* c = findCustomer(id);
        if (!c) { cout << "Customer not found." << endl; return; }
        c->display();
    }

    void listAllCustomers() {
        if (customers.empty()) { cout << "No customers yet." << endl; return; }
        for (auto& c : customers) {
            cout << "ID: " << c.id << " | Name: " << c.name << " | Balance: $" << c.account.getBalance() << endl;
        }
    }

    //save every customer + their balance to a text file (transaction
    //history is not saved here to keep the file format simple)
    void saveToFile() {
        ofstream file(DATA_FILE);
        for (auto& c : customers) {
            file << c.id << "," << c.name << "," << c.account.getAccountNumber() << "," << c.account.getBalance() << "\n";
        }
        file.close();
        cout << "Data saved to " << DATA_FILE << endl;
    }

    void loadFromFile() {
        ifstream file(DATA_FILE);
        if (!file.is_open()) return; //no saved data yet, that's fine

        string line;
        while (getline(file, line)) 
        {
            stringstream ss(line);
            string idStr, name, accNumStr, balanceStr;
            getline(ss, idStr, ',');
            getline(ss, name, ',');
            getline(ss, accNumStr, ',');
            getline(ss, balanceStr, ',');

            int id = stoi(idStr);
            int accNum = stoi(accNumStr);
            double balance = stod(balanceStr);

            customers.push_back(Customer(id, name, accNum, balance));

            if (id >= nextCustomerId) nextCustomerId = id + 1;
            if (accNum >= nextAccountNumber) nextAccountNumber = accNum + 1;
        }
        file.close();
        cout << "Loaded " << customers.size() << " customer(s) from file." << endl;
    }
};

int main() {
    Bank bank;
    bank.loadFromFile();

    int choice;
    do {
        cout << "===== BANKING SYSTEM =====" << endl;
        cout << "1. Add customer" << endl;
        cout << "2. Deposit" << endl;
        cout << "3. Withdraw" << endl;
        cout << "4. Transfer" << endl;
        cout << "5. View one customer" << endl;
        cout << "6. List all customers" << endl;
        cout << "7. Save and Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            string name;
            double startingBalance;
            cout << "Enter customer name: ";
            cin >> name;

            cout << "Enter starting balance: ";
            cin >> startingBalance;

            bank.addCustomer(name, startingBalance);

        } else if (choice == 2) {
            int id; double amount;
            cout << "Customer ID: "; cin >> id;
            cout << "Amount: "; cin >> amount;

            bank.deposit(id, amount);

        } else if (choice == 3) {
            int id; double amount;
            cout << "Customer ID: "; cin >> id;
            cout << "Amount: "; cin >> amount;

            bank.withdraw(id, amount);

        } else if (choice == 4) {
            int fromId, toId; double amount;
            cout << "From Customer ID: "; cin >> fromId;
            cout << "To Customer ID: "; cin >> toId;
            cout << "Amount: "; cin >> amount;

            bank.transfer(fromId, toId, amount);

        } else if (choice == 5) {
            int id;
            cout << "Customer ID: "; cin >> id;

            bank.showCustomer(id);

        } else if (choice == 6) {
            bank.listAllCustomers();

        } else if (choice == 7) {
            bank.saveToFile();
            cout << "Goodbye!" << endl;

        } else {
            cout << "Invalid choice, try again." << endl;
        }

    } while (choice != 7);

    return 0;
}