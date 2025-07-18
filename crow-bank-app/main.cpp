#include "crow_all.h"
#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>
#include <filesystem>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <random>
#include <iomanip>
#include <limits>
#include <sstream>

using namespace std;

#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

//================================ OOP CONCEPTS IMPLEMENTATION ================================

// Base class demonstrating Inheritance and Polymorphism
class Person
{
protected: // Data Abstraction - protected members
    string name;
    string email;
    string phone;

public:
    // Constructor
    Person() : name(""), email(""), phone("") {}

    Person(string n, string e, string p) : name(n), email(e), phone(p) {}

    // Virtual destructor for proper inheritance
    virtual ~Person() {}

    // Pure virtual function - Abstract class concept
    virtual void displayInfo() const = 0;

    // Getter methods - Encapsulation
    string getName() const { return name; }
    string getEmail() const { return email; }
    string getPhone() const { return phone; }

    // Setter methods - Encapsulation
    void setName(const string &n) { name = n; }
    void setEmail(const string &e) { email = e; }
    void setPhone(const string &p) { phone = p; }
};

// Account class demonstrating Encapsulation
class Account
{
private:
    string accountNumber;
    string accountType;
    double balance;
    vector<string> transactionHistory; // STL container

    // Private method for account number generation
    string generateAccountNumber()
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<long long> dis(100000000000LL, 999999999999LL);
        return to_string(dis(gen));
    }

public:
    // Constructor with member initialization list
    Account(string type = "Savings") : accountType(type), balance(0.0)
    {
        accountNumber = generateAccountNumber();
    }

    // Copy constructor
    Account(const Account &other)
        : accountNumber(other.accountNumber), accountType(other.accountType),
          balance(other.balance), transactionHistory(other.transactionHistory) {}

    // Assignment operator overloading
    Account &operator=(const Account &other)
    {
        if (this != &other)
        {
            accountNumber = other.accountNumber;
            accountType = other.accountType;
            balance = other.balance;
            transactionHistory = other.transactionHistory;
        }
        return *this;
    }

    // Operator overloading for deposit (+)
    Account &operator+=(double amount)
    {
        if (amount > 0)
        {
            balance += amount;
            stringstream ss;
            ss << fixed << setprecision(2) << amount;
            transactionHistory.push_back("Deposit: $" + ss.str());
        }
        return *this;
    }

    // Operator overloading for withdrawal (-)
    Account &operator-=(double amount)
    {
        if (amount > 0 && balance >= amount)
        {
            balance -= amount;
            stringstream ss;
            ss << fixed << setprecision(2) << amount;
            transactionHistory.push_back("Withdrawal: $" + ss.str());
        }
        return *this;
    }

    // Getter methods
    string getAccountNumber() const { return accountNumber; }
    string getAccountType() const { return accountType; }
    double getBalance() const { return balance; }
    vector<string> getTransactionHistory() const { return transactionHistory; }

    // Setter methods for file loading
    void setBalance(double newBalance) { balance = newBalance; }
    void setAccountNumber(const string &newAccountNumber) { accountNumber = newAccountNumber; }
    void setTransactionHistory(const vector<string> &history) { transactionHistory = history; }

    // Method for checking sufficient funds
    bool hasSufficientFunds(double amount) const
    {
        return balance >= amount;
    }
};

// Transaction class demonstrating Composition
class Transaction
{
private:
    string transactionId;
    string type;
    double amount;
    string timestamp;

    string generateTransactionId()
    {
        return "TXN" + to_string(time(nullptr)) + to_string(rand() % 1000);
    }

public:
    // Constructor for new transactions
    Transaction(string t, double a) : type(t), amount(a)
    {
        transactionId = generateTransactionId();
        time_t now = time(0);
        timestamp = ctime(&now);
        timestamp.pop_back(); // Remove newline
    }

    // Constructor for loading transactions from file
    Transaction(string id, string t, double a, string ts)
        : transactionId(id), type(t), amount(a), timestamp(ts) {}

    // Getter methods
    string getId() const { return transactionId; }
    string getType() const { return type; }
    double getAmount() const { return amount; }
    string getTimestamp() const { return timestamp; }
};

// User class demonstrating Inheritance from Person
class User : public Person
{
private:
    string password;
    Account *account;                 // Composition - User HAS-A Account
    vector<Transaction> transactions; // STL container for transactions
    static string bankName;           // Static member

public:
    // Constructor
    User() : Person(), password(""), account(nullptr) {}

    User(string n, string e, string p, string pass)
        : Person(n, e, p), password(pass)
    {
        account = new Account("Savings"); // Dynamic memory allocation
    }

    // Destructor
    ~User()
    {
        delete account; // Clean up dynamic memory
    }

    // Copy constructor
    User(const User &other) : Person(other), password(other.password)
    {
        if (other.account)
        {
            account = new Account(*other.account);
        }
        else
        {
            account = nullptr;
        }
        transactions = other.transactions;
    }

    // Assignment operator
    User &operator=(const User &other)
    {
        if (this != &other)
        {
            Person::operator=(other);
            password = other.password;
            delete account;
            if (other.account)
            {
                account = new Account(*other.account);
            }
            else
            {
                account = nullptr;
            }
            transactions = other.transactions;
        }
        return *this;
    }

    // Override pure virtual function from Person
    void displayInfo() const override
    {
        if (account)
        {
            cout << "Account: " << account->getAccountNumber()
                 << endl
                 << "Balance: $" << account->getBalance() << endl;
        }
        cout << "User Info - \nName: " << name << endl
             << "Email: " << email
             << endl
             << "Phone: " << phone << endl;
    }

    // Method to verify password
    bool verifyPassword(const string &pass) const
    {
        return password == pass;
    }

    // Methods for banking operations
    bool makeDeposit(double amount)
    {
        if (amount > 0 && account)
        {
            *account += amount; // Using overloaded operator
            transactions.push_back(Transaction("Deposit", amount));
            return true;
        }
        return false;
    }

    bool makeWithdrawal(double amount)
    {
        if (amount > 0 && account && account->hasSufficientFunds(amount))
        {
            *account -= amount; // Using overloaded operator
            transactions.push_back(Transaction("Withdrawal", amount));
            return true;
        }
        return false;
    }

    // Getter methods
    Account *getAccount() const { return account; }
    vector<Transaction> getTransactions() const { return transactions; }
    string getPassword() const { return password; } // For file storage

    // Setter method for loading detailed transactions from file
    void setTransactions(const vector<Transaction> &trans) { transactions = trans; }

    // Static method
    static void displayBankInfo()
    {
        cout << "Welcome to " << bankName << " Banking System" << endl;
    }

    // Friend function for accessing private members
    friend class BankSystem;
};

// Static member definition
string User::bankName = "SecureBank";

//================================ UTILITY FUNCTIONS ================================

// Cross-platform screen clearing function
void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

string get_executable_dir()
{
#ifdef _WIN32
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    string exe_path(path);
    size_t last_slash = exe_path.find_last_of("\\/");
    return exe_path.substr(0, last_slash);
#else
    string exe_path = filesystem::canonical("/proc/self/exe");
    return filesystem::path(exe_path).parent_path();
#endif
}

string get_file_path(const string &relative_path)
{
    static string exe_dir = get_executable_dir();
    return exe_dir + "/" + relative_path;
}

// Cross-platform icon display
string getIcon(const string &iconType)
{
#ifdef _WIN32
    // Windows - Smart emoji detection
    // Check for Windows Terminal or modern terminal capabilities
    static bool useEmojis = []()
    {
        // Check environment variables that indicate modern terminal
        const char *wt_session = getenv("WT_SESSION");     // Windows Terminal
        const char *term_program = getenv("TERM_PROGRAM"); // VS Code terminal, etc.
        const char *colorterm = getenv("COLORTERM");       // Modern terminals
        const char *force_emojis = getenv("FORCE_EMOJIS"); // User override
        const char *no_emojis = getenv("NO_EMOJIS");       // User disable

        // User explicitly disabled emojis
        if (no_emojis)
            return false;

        // User explicitly enabled emojis
        if (force_emojis)
            return true;

        // Detect modern terminal environment
        return (wt_session || term_program || colorterm);
    }();

    if (useEmojis)
    {
        // Unicode emojis for modern Windows terminals
        if (iconType == "bank")
            return "🏦";
        if (iconType == "register")
            return "📝";
        if (iconType == "login")
            return "🔐";
        if (iconType == "help")
            return "❓";
        if (iconType == "exit")
            return "🚪";
        if (iconType == "deposit")
            return "💰";
        if (iconType == "withdraw")
            return "💸";
        if (iconType == "balance")
            return "💳";
        if (iconType == "history")
            return "📊";
        if (iconType == "transfer")
            return "🔄";
        if (iconType == "settings")
            return "⚙️";
        if (iconType == "logout")
            return "🚪";
        if (iconType == "success")
            return "✅";
        if (iconType == "error")
            return "❌";
        if (iconType == "warning")
            return "⚠️";
        return "•";
    }
    else
    {
        // Windows fallback - basic ASCII characters only
        if (iconType == "bank")
            return "";
        if (iconType == "register")
            return "";
        if (iconType == "login")
            return "";
        if (iconType == "help")
            return "";
        if (iconType == "exit")
            return "";
        if (iconType == "deposit")
            return "";
        if (iconType == "withdraw")
            return "";
        if (iconType == "balance")
            return "";
        if (iconType == "history")
            return "";
        if (iconType == "transfer")
            return "";
        if (iconType == "settings")
            return "";
        if (iconType == "logout")
            return "";
        if (iconType == "success")
            return "[OK]";
        if (iconType == "error")
            return "[ERROR]";
        if (iconType == "warning")
            return "[WARNING]";
        return "";
    }
#else
    // Linux/Unix - Unicode emojis
    if (iconType == "bank")
        return "🏦";
    if (iconType == "register")
        return "📝";
    if (iconType == "login")
        return "🔐";
    if (iconType == "help")
        return "❓";
    if (iconType == "exit")
        return "🚪";
    if (iconType == "deposit")
        return "💰";
    if (iconType == "withdraw")
        return "💸";
    if (iconType == "balance")
        return "💳";
    if (iconType == "history")
        return "📊";
    if (iconType == "transfer")
        return "🔄";
    if (iconType == "settings")
        return "⚙️";
    if (iconType == "logout")
        return "🚪";
    if (iconType == "success")
        return "✅";
    if (iconType == "error")
        return "❌";
    if (iconType == "warning")
        return "⚠️";
    return "•";
#endif
}

// File storage functions
string getUserDataPath()
{
    return get_file_path("users.dat");
}

// Utility function to trim whitespace and carriage returns (Windows compatibility)
string trim(const string &str)
{
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == string::npos)
        return "";
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

// Simple encryption for password storage (basic XOR - not production-ready)
string encryptPassword(const string &password)
{
    string encrypted = password;
    const string key = "SecureBankKey2025";
    for (size_t i = 0; i < encrypted.length(); ++i)
    {
        encrypted[i] ^= key[i % key.length()];
    }

#ifdef _WIN32
    // Convert to hex string for Windows compatibility
    stringstream hex_stream;
    for (unsigned char c : encrypted)
    {
        hex_stream << hex << setfill('0') << setw(2) << (int)c;
    }
    return hex_stream.str();
#else
    return encrypted;
#endif
}

string decryptPassword(const string &encrypted)
{
#ifdef _WIN32
    // Convert from hex string on Windows
    if (encrypted.length() % 2 != 0)
    {
        // If not hex format, fall back to direct XOR (for backward compatibility)
        return encryptPassword(encrypted);
    }

    string binary_encrypted;
    for (size_t i = 0; i < encrypted.length(); i += 2)
    {
        string hex_byte = encrypted.substr(i, 2);
        try
        {
            unsigned char byte = static_cast<unsigned char>(stoi(hex_byte, nullptr, 16));
            binary_encrypted += byte;
        }
        catch (...)
        {
            // If hex conversion fails, fall back to direct XOR
            return encryptPassword(encrypted);
        }
    }

    // Now decrypt the binary data
    const string key = "SecureBankKey2025";
    for (size_t i = 0; i < binary_encrypted.length(); ++i)
    {
        binary_encrypted[i] ^= key[i % key.length()];
    }
    return binary_encrypted;
#else
    return encryptPassword(encrypted); // XOR is symmetric on Linux
#endif
}

// Secure password input function with masking
string getPasswordInput(const string &prompt)
{
    cout << prompt;
    string password;

#ifdef _WIN32
    // Windows implementation using _getch()
    char ch;
    while ((ch = _getch()) != '\r') // Enter key
    {
        if (ch == '\b') // Backspace
        {
            if (!password.empty())
            {
                password.pop_back();
                cout << "\b \b"; // Move back, print space, move back again
            }
        }
        else if (ch >= 32 && ch <= 126) // Printable characters
        {
            password += ch;
            cout << '*'; // Show asterisk instead of actual character
        }
    }
#else
    // Linux/Unix implementation using termios
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt); // Get current terminal settings
    newt = oldt;
    newt.c_lflag &= ~(ECHO);                 // Disable echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Apply new settings

    char ch;
    while ((ch = getchar()) != '\n') // Enter key
    {
        if (ch == 127 || ch == '\b') // Backspace (127 is DEL, \b is backspace)
        {
            if (!password.empty())
            {
                password.pop_back();
                cout << "\b \b"; // Move back, print space, move back again
            }
        }
        else if (ch >= 32 && ch <= 126) // Printable characters
        {
            password += ch;
            cout << '*'; // Show asterisk instead of actual character
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restore original terminal settings
#endif

    cout << endl; // Move to next line
    return password;
}

// Forward declaration for User class
class User;

// Forward declarations for functions
void showOOPConcepts();

// Save user data to file
bool saveUsersToFile(const map<string, User> &users)
{
    try
    {
        ofstream file(getUserDataPath(), ios::out);
        if (!file.is_open())
        {
            return false;
        }

        // Enhanced format: email|name|phone|encrypted_password|account_number|account_type|balance|transaction_history_count|transaction_history|detailed_transactions_count|detailed_transactions
        for (const auto &pair : users)
        {
            const User &user = pair.second;
            if (user.getAccount())
            {
                file << user.getEmail() << "|"
                     << user.getName() << "|"
                     << user.getPhone() << "|"
                     << encryptPassword(user.getPassword()) << "|"
                     << user.getAccount()->getAccountNumber() << "|"
                     << user.getAccount()->getAccountType() << "|"
                     << fixed << setprecision(2) << user.getAccount()->getBalance() << "|";

                // Save basic transaction history
                auto history = user.getAccount()->getTransactionHistory();
                file << history.size() << "|";
                for (const auto &trans : history)
                {
                    file << trans << ";";
                }
                file << "|";

                // Save detailed transactions
                auto detailedTransactions = user.getTransactions();
                file << detailedTransactions.size() << "|";
                for (const auto &trans : detailedTransactions)
                {
                    file << trans.getId() << "~" << trans.getType() << "~"
                         << fixed << setprecision(2) << trans.getAmount() << "~"
                         << trans.getTimestamp() << ";";
                }

                file << "\n"; // Use explicit \n instead of endl
            }
        }
        file.close();
        return true;
    }
    catch (...)
    {
        return false;
    }
}

// Load user data from file
bool loadUsersFromFile(map<string, User> &users, int &totalUsers)
{
    try
    {
        ifstream file(getUserDataPath());
        if (!file.is_open())
        {
            return false; // File doesn't exist yet
        }

        string line;
        while (getline(file, line))
        {
            if (line.empty())
                continue;

            stringstream ss(line);
            string email, name, phone, encryptedPass, accountNum, accountType, balanceStr, transCountStr, transactionsStr;

            if (getline(ss, email, '|') &&
                getline(ss, name, '|') &&
                getline(ss, phone, '|') &&
                getline(ss, encryptedPass, '|') &&
                getline(ss, accountNum, '|') &&
                getline(ss, accountType, '|') &&
                getline(ss, balanceStr, '|'))
            {
                // Trim all fields to remove carriage returns (Windows compatibility)
                email = trim(email);
                name = trim(name);
                phone = trim(phone);
                encryptedPass = trim(encryptedPass);
                accountNum = trim(accountNum);
                accountType = trim(accountType);
                balanceStr = trim(balanceStr);

                string password = decryptPassword(encryptedPass);
                password = trim(password); // Trim decrypted password to remove any carriage returns
                double balance = stod(balanceStr);

                User user(name, email, phone, password);
                // Set account details
                if (user.getAccount())
                {
                    user.getAccount()->setBalance(balance);
                    user.getAccount()->setAccountNumber(accountNum);

                    // Load basic transaction history if available (for backward compatibility)
                    if (getline(ss, transCountStr, '|') && getline(ss, transactionsStr, '|'))
                    {
                        transCountStr = trim(transCountStr);
                        transactionsStr = trim(transactionsStr);

                        int transCount = stoi(transCountStr);
                        if (transCount > 0)
                        {
                            vector<string> history;
                            stringstream transSS(transactionsStr);
                            string transaction;
                            while (getline(transSS, transaction, ';') && !transaction.empty())
                            {
                                history.push_back(trim(transaction));
                            }
                            user.getAccount()->setTransactionHistory(history);
                        }
                    }

                    // Load detailed transactions if available
                    string detailedCountStr, detailedTransStr;
                    if (getline(ss, detailedCountStr, '|') && getline(ss, detailedTransStr))
                    {
                        detailedCountStr = trim(detailedCountStr);
                        detailedTransStr = trim(detailedTransStr);

                        int detailedCount = stoi(detailedCountStr);
                        if (detailedCount > 0)
                        {
                            vector<Transaction> detailedTransactions;
                            stringstream detailedSS(detailedTransStr);
                            string detailedTransaction;

                            while (getline(detailedSS, detailedTransaction, ';') && !detailedTransaction.empty())
                            {
                                stringstream transFieldSS(detailedTransaction);
                                string id, type, amountStr, timestamp;

                                if (getline(transFieldSS, id, '~') &&
                                    getline(transFieldSS, type, '~') &&
                                    getline(transFieldSS, amountStr, '~') &&
                                    getline(transFieldSS, timestamp))
                                {
                                    double amount = stod(trim(amountStr));
                                    detailedTransactions.push_back(
                                        Transaction(trim(id), trim(type), amount, trim(timestamp)));
                                }
                            }
                            user.setTransactions(detailedTransactions);
                        }
                    }
                }

                users[email] = user;
                totalUsers++;
            }
        }
        file.close();
        return true;
    }
    catch (...)
    {
        return false;
    }
}

// Bank System class demonstrating Aggregation and Templates
class BankSystem
{
private:
    string bankName;
    map<string, User> users;      // STL container - Aggregation
    map<string, string> sessions; // token -> email mapping
    static int totalUsers;        // Static member

    string generateToken()
    {
        return "token_" + to_string(rand()) + "_" + to_string(time(nullptr));
    }

public:
    // Constructor
    BankSystem(string name) : bankName(name)
    {
        // Load existing users from file
        loadUsersFromFile(users, totalUsers);
    }

    // Destructor - save data when system shuts down
    ~BankSystem()
    {
        saveUsersToFile(users);
    }

    // Method to authenticate user
    string authenticateUser(const string &email, const string &password)
    {
        reloadData(); // Reload fresh data from file

        // Trim input parameters to handle any extra whitespace
        string trimmedEmail = trim(email);
        string trimmedPassword = trim(password);

        auto it = users.find(trimmedEmail);
        if (it != users.end() && it->second.verifyPassword(trimmedPassword))
        {
            string token = generateToken();
            sessions[token] = trimmedEmail;
            return token;
        }
        return "";
    }

    // Method to get user by token
    User *getUserByToken(const string &token)
    {
        reloadData(); // Reload fresh data from file
        auto sessionIt = sessions.find(token);
        if (sessionIt != sessions.end())
        {
            auto userIt = users.find(sessionIt->second);
            if (userIt != users.end())
            {
                return &userIt->second;
            }
        }
        return nullptr;
    }

    // Method to add new user
    bool addUser(const User &user)
    {
        if (users.find(user.getEmail()) == users.end())
        {
            users[user.getEmail()] = user;
            totalUsers++;
            saveUsersToFile(users); // Save immediately
            return true;
        }
        return false;
    }

    // Method to save current state
    bool saveData()
    {
        return saveUsersToFile(users);
    }

    // Method to reload data from file (for sync between CLI and web)
    bool reloadData()
    {
        users.clear(); // Clear current in-memory data
        totalUsers = 0;
        return loadUsersFromFile(users, totalUsers);
    }

    // Static method to get total users
    static int getTotalUsers()
    {
        return totalUsers;
    }

    // Template method for finding users (Template concept)
    template <typename T>
    User *findUser(const string &key, T value)
    {
        reloadData(); // Reload fresh data from file
        for (auto &pair : users)
        {
            if (key == "email" && pair.second.getEmail() == value)
            {
                return &pair.second;
            }
            if (key == "name" && pair.second.getName() == value)
            {
                return &pair.second;
            }
        }
        return nullptr;
    }
};

// Static member definition
int BankSystem::totalUsers = 0;

//================================ WEB UTILITY FUNCTIONS ================================

void open_browser(const string &url)
{
#ifdef _WIN32
    ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
#else
    string command = "xdg-open " + url + " 2>/dev/null &";
    system(command.c_str());
#endif
}

string read_file(const string &filename)
{
    string file_path = get_file_path("public/" + filename);
    ifstream file(file_path);
    if (!file.is_open())
    {
        return "Error: Could not load " + filename + ". Make sure public folder exists in the same directory as the executable.";
    }
    return string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
}

//================================ CLI BANKING SYSTEM ================================

// Main CLI authentication menu
void displayAuthMenu()
{
    cout << "Made with <3\n\n";
    cout << "==============================================\n";
    cout << "        " << getIcon("bank") << " SECURE BANK CLI SYSTEM        \n";
    cout << "==============================================\n";
    cout << "\n";
    cout << "1. " << getIcon("register") << "Register New Account\n";
    cout << "2. " << getIcon("login") << "Login to Account\n";
    cout << "3. " << getIcon("help") << "Help & OOP Concepts\n";
    cout << "4. " << getIcon("exit") << "Exit\n";
    cout << "\n==============================================\n";
    cout << "\nSelect option (1-4): ";
}

// Banking dashboard menu (after login)
void displayBankingMenu(const string &userName)
{
    cout << "Made with <3\n\n";
    cout << "==============================================\n";
    cout << "      " << getIcon("bank") << " WELCOME " << userName << "\n";
    cout << "==============================================\n";
    cout << "         Banking Dashboard\n";
    cout << "==============================================\n";
    cout << "\n";
    cout << "1. " << getIcon("deposit") << "Deposit Money\n";
    cout << "2. " << getIcon("withdraw") << "Withdraw Money\n";
    cout << "3. " << getIcon("balance") << "Check Balance\n";
    cout << "4. " << getIcon("history") << "Transaction History\n";
    cout << "5. " << getIcon("transfer") << "Transfer Money\n";
    cout << "6. " << getIcon("settings") << "Account Settings\n";
    cout << "7. " << getIcon("logout") << "Logout\n";
    cout << "\n==============================================\n";
    cout << "\nSelect option (1-7): ";
}

// Registration function
bool registerNewUser(BankSystem &bank)
{
    cout << "\n=== " << getIcon("register") << " REGISTER NEW ACCOUNT ===\n";

    string firstName, lastName, email, phone, password, confirmPassword;

    cout << "Enter First Name: ";
    getline(cin, firstName);

    cout << "Enter Last Name: ";
    getline(cin, lastName);

    string fullName = firstName + " " + lastName;

    cout << "Enter Email: ";
    getline(cin, email);

    // Email validation
    if (email.find('@') == string::npos || email.find('.') == string::npos)
    {
        cout << getIcon("error") << " Invalid email format!\n";
        return false;
    }

    cout << "Enter Phone Number: ";
    getline(cin, phone);

    password = getPasswordInput("Enter Password: ");

    confirmPassword = getPasswordInput("Confirm Password: ");

    if (password != confirmPassword)
    {
        cout << getIcon("error") << " Passwords do not match!\n";
        return false;
    }

    if (password.length() < 6)
    {
        cout << getIcon("error") << " Password must be at least 6 characters!\n";
        return false;
    }

    // Create new user
    User newUser(fullName, email, phone, password);

    if (bank.addUser(newUser))
    {
        cout << "\n"
             << getIcon("success") << " Account created successfully!\n";
        // cout << "Name: " << fullName << "\n";
        // cout << "Email: " << email << "\n";
        // cout << "Account Number: " << newUser.getAccount()->getAccountNumber() << "\n";
        // cout << "Account Type: " << newUser.getAccount()->getAccountType() << "\n";
        // cout << "Initial Balance: $0.00\n";
        cout << "\nYou can now login with your email and password.\n";
        return true;
    }
    else
    {
        cout << getIcon("error") << " Registration failed! Email already exists.\n";
        return false;
    }
}

// Login function
User *loginUser(BankSystem &bank)
{
    cout << "\n=== " << getIcon("login") << " LOGIN TO ACCOUNT ===\n";

    string email, password;

    cout << "Enter Email: ";
    getline(cin, email);

    password = getPasswordInput("Enter Password: ");

    string token = bank.authenticateUser(email, password);

    if (!token.empty())
    {
        User *user = bank.getUserByToken(token);
        if (user)
        {
            cout << "\n"
                 << getIcon("success") << " Login successful!\n";
            cout << "Welcome back, " << user->getName() << "!\n";
            return user;
        }
    }

    cout << getIcon("error") << " Invalid email or password!\n";
    return nullptr;
}

// Banking operations after login
void handleBankingOperations(BankSystem &bank, User *currentUser)
{
    int choice;
    string userEmail = currentUser->getEmail(); // Store email for reloading

    while (true)
    {
        // Refresh user data from file before each operation
        currentUser = bank.findUser("email", userEmail);
        if (!currentUser)
        {
            cout << getIcon("error") << " User session expired. Please login again.\n";
            return;
        }

        clearScreen(); // Clear screen for clean banking interface
        displayBankingMenu(currentUser->getName());

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << getIcon("error") << " Invalid input! Please enter a number (1-7).\n";
            cout << "Press Enter to continue...";
            cin.get();
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice)
        {
        case 1:
        { // Deposit Money
            clearScreen();
            cout << "\n=== " << getIcon("deposit") << " DEPOSIT MONEY ===\n";
            cout << "Current Balance: $" << fixed << setprecision(2) << currentUser->getAccount()->getBalance() << "\n";
            cout << "Enter Amount to Deposit: $";
            double amount;
            cin >> amount;
            cin.ignore(); // Clear input buffer

            if (amount > 0)
            {
                if (currentUser->makeDeposit(amount))
                {
                    bank.saveData(); // Save after transaction
                    cout << getIcon("success") << " Deposit successful!\n";
                    cout << "New Balance: $" << fixed << setprecision(2) << currentUser->getAccount()->getBalance() << "\n";
                }
                else
                {
                    cout << getIcon("error") << " Deposit failed!\n";
                }
            }
            else
            {
                cout << getIcon("error") << " Invalid amount!\n";
            }
            cout << "\nPress Enter to continue...";
            cin.get();
            break;
        }

        case 2:
        { // Withdraw Money
            clearScreen();
            cout << "\n=== " << getIcon("withdraw") << " WITHDRAW MONEY ===\n";
            cout << "Current Balance: $" << fixed << setprecision(2) << currentUser->getAccount()->getBalance() << "\n";
            cout << "Enter Amount to Withdraw: $";
            double amount;
            cin >> amount;
            cin.ignore(); // Clear input buffer

            if (amount > 0)
            {
                if (currentUser->makeWithdrawal(amount))
                {
                    bank.saveData(); // Save after transaction
                    cout << getIcon("success") << " Withdrawal successful!\n";
                    cout << "New Balance: $" << fixed << setprecision(2) << currentUser->getAccount()->getBalance() << "\n";
                }
                else
                {
                    cout << getIcon("error") << " Withdrawal failed! Insufficient funds or invalid amount.\n";
                }
            }
            else
            {
                cout << getIcon("error") << " Invalid amount!\n";
            }
            cout << "\nPress Enter to continue...";
            cin.get();
            break;
        }

        case 3:
        { // Check Balance
            clearScreen();
            cout << "\n=== " << getIcon("balance") << " ACCOUNT BALANCE ===\n";
            cout << "Account Holder: " << currentUser->getName() << "\n";
            cout << "Current Balance: $" << fixed << setprecision(2) << currentUser->getAccount()->getBalance() << "\n";
            cout << "\nPress Enter to continue...";
            cin.get();
            break;
        }

        case 4:
        { // Transaction History
            clearScreen();
            cout << "\n=== " << getIcon("history") << " TRANSACTION HISTORY ===\n";
            auto transactions = currentUser->getTransactions();
            auto accountHistory = currentUser->getAccount()->getTransactionHistory();

            cout << "Account: " << currentUser->getAccount()->getAccountNumber() << "\n";
            cout << "Current Balance: $" << fixed << setprecision(2) << currentUser->getAccount()->getBalance() << "\n\n";

            cout << "Recent Transactions:\n";
            for (const auto &trans : accountHistory)
            {
                cout << "- " << trans << "\n";
            }

            cout << "\nDetailed Transaction Log:\n";
            for (const auto &trans : transactions)
            {
                cout << "ID: " << trans.getId()
                     << " | Type: " << trans.getType()
                     << " | Amount: $" << fixed << setprecision(2) << trans.getAmount()
                     << " | Time: " << trans.getTimestamp() << "\n";
            }

            if (transactions.empty() && accountHistory.empty())
            {
                cout << "No transactions found.\n";
            }
            cout << "\nPress Enter to continue...";
            cin.get();
            break;
        }

        case 5:
        { // Transfer Money
            clearScreen();
            cout << "\n=== " << getIcon("transfer") << " TRANSFER MONEY ===\n";
            cout << "Your Balance: $" << fixed << setprecision(2) << currentUser->getAccount()->getBalance() << "\n";
            cout << "Enter Recipient Email: ";
            string toEmail;
            getline(cin, toEmail);

            User *toUser = bank.findUser("email", toEmail);
            if (!toUser)
            {
                cout << getIcon("error") << " Recipient account not found!\n";
                cout << "\nPress Enter to continue...";
                cin.get();
                break;
            }

            if (toUser->getEmail() == currentUser->getEmail())
            {
                cout << getIcon("error") << " Cannot transfer to yourself!\n";
                cout << "\nPress Enter to continue...";
                cin.get();
                break;
            }

            cout << "Recipient: " << toUser->getName() << "\n";
            cout << "Enter Amount to Transfer: $";
            double amount;
            cin >> amount;
            cin.ignore(); // Clear input buffer

            if (amount > 0)
            {
                if (currentUser->makeWithdrawal(amount))
                {
                    if (toUser->makeDeposit(amount))
                    {
                        bank.saveData(); // Save after transaction
                        cout << getIcon("success") << " Transfer successful!\n";
                        cout << "Transferred $" << fixed << setprecision(2) << amount << " to " << toUser->getName() << "\n";
                        cout << "Your new balance: $" << fixed << setprecision(2) << currentUser->getAccount()->getBalance() << "\n";
                    }
                    else
                    {
                        // Rollback if deposit fails
                        currentUser->makeDeposit(amount);
                        cout << getIcon("error") << " Transfer failed! Deposit to recipient failed.\n";
                    }
                }
                else
                {
                    cout << getIcon("error") << " Transfer failed! Insufficient funds.\n";
                }
            }
            else
            {
                cout << getIcon("error") << " Invalid amount!\n";
            }
            cout << "\nPress Enter to continue...";
            cin.get();
            break;
        }

        case 6:
        { // Account Settings
            clearScreen();
            cout << "\n=== " << getIcon("settings") << " ACCOUNT SETTINGS ===\n";
            cout << "1. View Profile\n";
            cout << "2. Change Password\n";
            cout << "3. Update Phone Number\n";
            cout << "4. Back to Dashboard\n";
            cout << "Select option (1-4): ";

            int settingsChoice;
            cin >> settingsChoice;
            cin.ignore(); // Clear input buffer

            switch (settingsChoice)
            {
            case 1:
            {
                clearScreen();
                cout << "\n--- PROFILE INFORMATION ---\n\n";
                currentUser->displayInfo();
                cout << "\nPress Enter to continue...";
                cin.get();
                break;
            }
            case 2:
            {
                clearScreen();
                cout << "\n--- CHANGE PASSWORD ---\n";
                string currentPass, newPass, confirmPass;
                currentPass = getPasswordInput("Enter Current Password: ");

                if (!currentUser->verifyPassword(currentPass))
                {
                    cout << getIcon("error") << " Current password is incorrect!\n";
                    cout << "\nPress Enter to continue...";
                    cin.get();
                    break;
                }

                newPass = getPasswordInput("Enter New Password: ");
                confirmPass = getPasswordInput("Confirm New Password: ");

                if (newPass != confirmPass)
                {
                    cout << getIcon("error") << " Passwords do not match!\n";
                }
                else if (newPass.length() < 6)
                {
                    cout << getIcon("error") << " Password must be at least 6 characters!\n";
                }
                else
                {
                    // Note: In a full implementation, we'd add a setPassword method
                    cout << getIcon("success") << " Password change simulated (feature not fully implemented).\n";
                }
                cout << "\nPress Enter to continue...";
                cin.get();
                break;
            }
            case 3:
            {
                clearScreen();
                cout << "\n--- UPDATE PHONE NUMBER ---\n";
                cout << "Current Phone: " << currentUser->getPhone() << "\n";
                cout << "Enter New Phone Number: ";
                string newPhone;
                getline(cin, newPhone);

                currentUser->setPhone(newPhone);
                bank.saveData();
                cout << getIcon("success") << " Phone number updated successfully!\n";
                cout << "\nPress Enter to continue...";
                cin.get();
                break;
            }
            case 4:
                continue; // Back to dashboard
            default:
                cout << getIcon("error") << " Invalid choice!\n";
                cout << "\nPress Enter to continue...";
                cin.get();
                break;
            }
            break;
        }

        case 7:
        { // Logout
            clearScreen();
            cout << "\n"
                 << getIcon("logout") << " Logging out...\n";
            cout << "Thank you for using SecureBank, " << currentUser->getName() << "!\n";
            return; // Exit banking operations
        }

        default:
        {
            cout << getIcon("error") << " Invalid choice! Please select 1-7.\n";
            cout << "\nPress Enter to continue...";
            cin.get();
            break;
        }
        }
    }
}

void runCLI()
{
    BankSystem bank("SecureBank CLI");

    int choice;

    while (true)
    {
        clearScreen(); // Clear screen for clean interface
        displayAuthMenu();

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << getIcon("error") << " Invalid input! Please enter a number (1-4).\n";
            cout << "Press Enter to continue...";
            cin.get();
            continue;
        }

        // Clear the input buffer after successful input
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice)
        {
        case 1:
        { // Register
            clearScreen();
            if (registerNewUser(bank))
            {
                // Registration successful, continue to main menu for login
            }
            cout << "\nPress Enter to continue...";
            cin.get();
            break;
        }

        case 2:
        { // Login
            clearScreen();
            User *currentUser = loginUser(bank);
            if (currentUser)
            {
                // Start banking session
                handleBankingOperations(bank, currentUser);
            }
            cout << "\nPress Enter to continue...";
            cin.get();
            break;
        }

        case 3:
        { // Help
            showOOPConcepts();
            break;
        }

        case 4:
        { // Exit
            clearScreen();
            cout << "\n"
                 << getIcon("exit") << " Thank you for using SecureBank!\n";
            return;
        }

        default:
        {
            cout << getIcon("error") << " Invalid choice! Please select 1-4.\n";
            cout << "\nPress Enter to continue...";
            cin.get();
            break;
        }
        }
    }
}

//================================ MODE SELECTION ================================

void displayWelcomeScreen()
{
    cout << "Made with <3\n\n";
    cout << "========================================\n";
    cout << "         " << getIcon("bank") << " SECURE BANK SYSTEM         \n";
    cout << "========================================\n";
    cout << "\nAvailable Modes:\n";
    cout << "1. " << getIcon("register") << "CLI Mode  - Command Line Interface\n";
    cout << "2. " << getIcon("login") << "Web Mode  - Browser Interface\n";
    cout << "3. " << getIcon("exit") << "Exit      - Quit application\n";
    cout << "\n========================================\n";
    cout << "\nSelect mode (1-3): ";
}

void showOOPConcepts()
{
    clearScreen();
    cout << "\n"
         << getIcon("help") << " About This Banking System:\n";
    cout << "=========================================\n";
    cout << "\nThis project demonstrates various C++ concepts:\n\n";
    cout << "- Classes and Objects - User, Account, BankSystem classes\n";
    cout << "- Inheritance - User class inherits from Person base class\n";
    cout << "- Polymorphism - Virtual functions and method overriding\n";
    cout << "- Encapsulation - Private data with public methods\n";
    cout << "- File I/O - Saving user data to local files\n";
    cout << "- STL Containers - Using vector and map for data storage\n";
    cout << "- Error Handling - Input validation and exception handling\n";
    cout << "\nFeatures:\n";
    cout << "- Account registration and login\n";
    cout << "- Deposit and withdrawal operations\n";
    cout << "- Money transfer between accounts\n";
    cout << "- Transaction history tracking\n";
    cout << "- Cross-platform compatibility (Windows/Linux)\n";
    cout << "- Both CLI and Web interfaces\n";
    cout << "\nThis system shows practical application of OOP principles\n";
    cout << "in a real-world banking scenario.\n\n";
    cout << "@Prash <3\n";

    cout << "\nPress Enter to continue...";
    cin.get();
}

//================================ MAIN APPLICATION ================================

int main(int argc, char *argv[])
{
    // Quick command line support (for advanced users)
    if (argc > 1)
    {
        string arg = argv[1];
        if (arg == "--cli" || arg == "-c")
        {
            runCLI();
            return 0;
        }
        else if (arg == "--web" || arg == "-w")
        {
            // Skip menu, go directly to web mode
            cout << "Starting Web Banking Application directly...\n\n";
            goto web_mode;
        }
    }

    // Interactive Mode Selection
    int choice;
    while (true)
    {
        clearScreen(); // Clear screen for main menu
        displayWelcomeScreen();

        // Clear input buffer and read choice
        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n"
                 << getIcon("error") << " Invalid input! Please enter a number (1-3).\n";
            cout << "Press Enter to continue...";
            cin.get();
            continue;
        }

        // Clear remaining input buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice)
        {
        case 1:
        { // CLI Mode
            cout << "\nStarting CLI Banking System...\n";
            runCLI();
            return 0;
        }

        case 2:
        { // Web Mode
            cout << "\nStarting Web Banking Application...\n";
            break; // Exit switch to start web mode
        }

        case 3:
        { // Exit
            clearScreen();
            cout << "\n� Thank you for exploring SecureBank!\n";

            return 0;
        }

        default:
        {
            cout << "\n"
                 << getIcon("error") << " Invalid choice! Please select 1-3.\n";
            cout << "Press Enter to continue...";
            cin.get();
            continue;
        }
        }

        // If we reach here, user chose Web Mode (case 2)
        break;
    }

web_mode:
    // Create bank system object - Object creation
    BankSystem bank("SecureBank");

    // Display static information
    User::displayBankInfo();

    crow::SimpleApp app;
    string exe_dir = get_executable_dir();
    cout << "Executable directory: " << exe_dir << endl;

    // Check if public folder exists using absolute path
    string index_path = get_file_path("public/index.html");
    ifstream test_file(index_path);
    if (!test_file.is_open())
    {
        cerr << getIcon("warning") << " Warning: " << index_path << " not found!" << endl;
        cerr << "   This usually happens when Task Scheduler runs from wrong directory" << endl;
    }
    else
    {
        cout << getIcon("success") << " Found: " << index_path << endl;
        test_file.close();
    }

    // Route for the main page (default login page)
    CROW_ROUTE(app, "/")
    ([](const crow::request &req)
     { return crow::response(200, read_file("index.html")); });

    // Route for signup page
    CROW_ROUTE(app, "/signup")
    ([](const crow::request &req)
     { return crow::response(200, read_file("register.html")); });

    // Route for register page (alias for signup)
    CROW_ROUTE(app, "/register")
    ([](const crow::request &req)
     { return crow::response(200, read_file("register.html")); });

    // Route for login page (alias for root)
    CROW_ROUTE(app, "/login")
    ([](const crow::request &req)
     { return crow::response(200, read_file("index.html")); });

    // Route for dashboard page
    CROW_ROUTE(app, "/dashboard")
    ([](const crow::request &req)
     { return crow::response(200, read_file("dashboard.html")); });

    // API route for login - Using OOP objects
    CROW_ROUTE(app, "/api/login").methods("POST"_method)([&bank](const crow::request &req)
                                                         {
        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response(400, "{\"success\": false, \"message\": \"Invalid JSON\"}");
        }

        string email = body["email"].s();
        string password = body["password"].s();

        // Use bank system object for authentication
        string token = bank.authenticateUser(email, password);
        
        if (!token.empty()) {
            crow::json::wvalue response;
            response["success"] = true;
            response["token"] = token;
            response["message"] = "Login successful";
            return crow::response(200, response.dump());
        } else {
            crow::json::wvalue response;
            response["success"] = false;
            response["message"] = "Invalid email or password";
            return crow::response(401, response.dump());
        } });

    // API route for user data - Demonstrating object interaction
    CROW_ROUTE(app, "/api/user-data")
    ([&bank](const crow::request &req)
     {
        string auth_header = req.get_header_value("Authorization");
        if (auth_header.substr(0, 7) != "Bearer ") {
            return crow::response(401, "{\"error\": \"Unauthorized\"}");
        }
        
        string token = auth_header.substr(7);
        User* user = bank.getUserByToken(token);
        
        if (user && user->getAccount()) {
            crow::json::wvalue response;
            response["name"] = user->getName();
            response["email"] = user->getEmail();
            response["balance"] = user->getAccount()->getBalance();
            response["accountNumber"] = user->getAccount()->getAccountNumber();
            response["accountType"] = user->getAccount()->getAccountType();
            
            return crow::response(200, response.dump());
        } else {
            return crow::response(404, "{\"error\": \"User not found\"}");
        } });

    // API route for deposit
    CROW_ROUTE(app, "/api/deposit").methods("POST"_method)([&bank](const crow::request &req)
                                                           {
        auto body = crow::json::load(req.body);
        string auth_header = req.get_header_value("Authorization");
        
        if (auth_header.substr(0, 7) != "Bearer ") {
            return crow::response(401, "{\"error\": \"Unauthorized\"}");
        }
        
        string token = auth_header.substr(7);
        User* user = bank.getUserByToken(token);
        
        if (user && body) {
            double amount = body["amount"].d();
            if (user->makeDeposit(amount)) {
                bank.saveData(); // Save after successful deposit
                crow::json::wvalue response;
                response["success"] = true;
                response["message"] = "Deposit successful";
                response["newBalance"] = user->getAccount()->getBalance();
                return crow::response(200, response.dump());
            }
        }
        
        return crow::response(400, "{\"error\": \"Deposit failed\"}"); });

    // API route for withdrawal
    CROW_ROUTE(app, "/api/withdraw").methods("POST"_method)([&bank](const crow::request &req)
                                                            {
        auto body = crow::json::load(req.body);
        string auth_header = req.get_header_value("Authorization");
        
        if (auth_header.substr(0, 7) != "Bearer ") {
            return crow::response(401, "{\"error\": \"Unauthorized\"}");
        }
        
        string token = auth_header.substr(7);
        User* user = bank.getUserByToken(token);
        
        if (user && body) {
            double amount = body["amount"].d();
            if (user->makeWithdrawal(amount)) {
                bank.saveData(); // Save after successful withdrawal
                crow::json::wvalue response;
                response["success"] = true;
                response["message"] = "Withdrawal successful";
                response["newBalance"] = user->getAccount()->getBalance();
                return crow::response(200, response.dump());
            } else {
                return crow::response(400, "{\"error\": \"Insufficient funds or invalid amount\"}");
            }
        }
        
        return crow::response(400, "{\"error\": \"Withdrawal failed\"}"); });

    // API route for user registration
    CROW_ROUTE(app, "/api/register").methods("POST"_method)([&bank](const crow::request &req)
                                                            {
        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response(400, "{\"success\": false, \"message\": \"Invalid JSON\"}");
        }

        try {
            string name = body["name"].s();
            string email = body["email"].s();
            string phone = body["phone"].s();
            string password = body["password"].s();

            // Basic validation
            if (name.empty() || email.empty() || phone.empty() || password.empty()) {
                return crow::response(400, "{\"success\": false, \"message\": \"All fields are required\"}");
            }

            if (password.length() < 6) {
                return crow::response(400, "{\"success\": false, \"message\": \"Password must be at least 6 characters\"}");
            }

            if (email.find('@') == string::npos || email.find('.') == string::npos) {
                return crow::response(400, "{\"success\": false, \"message\": \"Invalid email format\"}");
            }

            // Create new user
            User newUser(name, email, phone, password);
            
            if (bank.addUser(newUser)) {
                crow::json::wvalue response;
                response["success"] = true;
                response["message"] = "Account created successfully";
                response["accountNumber"] = newUser.getAccount()->getAccountNumber();
                return crow::response(201, response.dump());
            } else {
                return crow::response(409, "{\"success\": false, \"message\": \"Email already exists\"}");
            }
        } catch (...) {
            return crow::response(400, "{\"success\": false, \"message\": \"Invalid request data\"}");
        } });

    // API route for transaction history
    CROW_ROUTE(app, "/api/transactions")
    ([&bank](const crow::request &req)
     {
        string auth_header = req.get_header_value("Authorization");
        if (auth_header.substr(0, 7) != "Bearer ") {
            return crow::response(401, "{\"error\": \"Unauthorized\"}");
        }
        
        string token = auth_header.substr(7);
        User* user = bank.getUserByToken(token);
        
        if (user && user->getAccount()) {
            crow::json::wvalue response;
            
            // Get account transaction history
            auto accountHistory = user->getAccount()->getTransactionHistory();
            response["accountTransactions"] = crow::json::wvalue::list();
            for (size_t i = 0; i < accountHistory.size(); i++) {
                response["accountTransactions"][i] = accountHistory[i];
            }
            
            // Get detailed transaction log
            auto transactions = user->getTransactions();
            response["detailedTransactions"] = crow::json::wvalue::list();
            for (size_t i = 0; i < transactions.size(); i++) {
                crow::json::wvalue trans;
                trans["id"] = transactions[i].getId();
                trans["type"] = transactions[i].getType();
                trans["amount"] = transactions[i].getAmount();
                trans["timestamp"] = transactions[i].getTimestamp();
                response["detailedTransactions"][i] = move(trans);
            }
            
            return crow::response(200, response.dump());
        } else {
            return crow::response(404, "{\"error\": \"User not found\"}");
        } });

    // API route for money transfer
    CROW_ROUTE(app, "/api/transfer").methods("POST"_method)([&bank](const crow::request &req)
                                                            {
        auto body = crow::json::load(req.body);
        string auth_header = req.get_header_value("Authorization");
        
        if (auth_header.substr(0, 7) != "Bearer ") {
            return crow::response(401, "{\"error\": \"Unauthorized\"}");
        }
        
        string token = auth_header.substr(7);
        User* fromUser = bank.getUserByToken(token);
        
        if (fromUser && body) {
            try {
                string toEmail = body["toEmail"].s();
                double amount = body["amount"].d();

                if (amount <= 0) {
                    return crow::response(400, "{\"error\": \"Invalid amount\"}");
                }

                User* toUser = bank.findUser("email", toEmail);
                if (!toUser) {
                    return crow::response(404, "{\"error\": \"Recipient account not found\"}");
                }

                if (toUser->getEmail() == fromUser->getEmail()) {
                    return crow::response(400, "{\"error\": \"Cannot transfer to yourself\"}");
                }

                // Perform transfer
                if (fromUser->makeWithdrawal(amount)) {
                    if (toUser->makeDeposit(amount)) {
                        bank.saveData(); // Save after successful transfer
                        
                        crow::json::wvalue response;
                        response["success"] = true;
                        response["message"] = "Transfer successful";
                        response["newBalance"] = fromUser->getAccount()->getBalance();
                        response["recipient"] = toUser->getName();
                        response["amount"] = amount;
                        return crow::response(200, response.dump());
                    } else {
                        // Rollback withdrawal if deposit fails
                        fromUser->makeDeposit(amount);
                        return crow::response(500, "{\"error\": \"Transfer failed - deposit error\"}");
                    }
                } else {
                    return crow::response(400, "{\"error\": \"Insufficient funds\"}");
                }
            } catch (...) {
                return crow::response(400, "{\"error\": \"Invalid request data\"}");
            }
        }
        
        return crow::response(400, "{\"error\": \"Transfer failed\"}"); });

    // Route for JavaScript files
    CROW_ROUTE(app, "/js/<path>")
    ([](const crow::request &req, const string &path)
     { return crow::response(200, read_file("js/" + path)); });

    // Route for CSS file
    CROW_ROUTE(app, "/style.css")
    ([](const crow::request &req)
     { return crow::response(200, read_file("style.css")); });

    // Start the server in a separate thread so we can open browser after startup
    thread server_thread([&app]()
                         { app.port(8080).multithreaded().run(); });

    // Wait a moment for server to start, then open browser
    this_thread::sleep_for(chrono::milliseconds(2000));
    cout << "Opening browser..." << endl;
    open_browser("http://localhost:8080");

    // Keep the main thread alive
    server_thread.join();

    return 0;
}
