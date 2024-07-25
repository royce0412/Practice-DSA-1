#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
using namespace std;

// Ensure the global filename is defined
string global_filename = "details.txt";

struct Person {
    string username;
    string password;
    string surname;
    string firstname;
    string middlename;
    string dateofbirth;
    string address;
    string civilStatus;
    string fatherFullname;
    string motherFullname;
    int income; // in PHP (Philippine Peso)
};

struct node {
    Person person;
    node* next;
};

class linked_list {
private:
    node* head;

public:
    linked_list() {
        head = nullptr;
    }

    ~linked_list() {
        clearList(); // Ensure the list is cleaned up when the linked_list object is destroyed
    }

    bool isUsernameTaken(const string& username) {
        ifstream readfile(global_filename);
        if (!readfile) {
            cerr << "Failed to open file." << endl;
            return false;
        }

        string line;
        while (getline(readfile, line)) {
            stringstream ss(line);
            string existingUsername;
            getline(ss, existingUsername, '|');

            if (existingUsername == username) {
                return true;
            }
        }

        return false;
    }

    void registerPerson(string file) {
        string choice;
        Person newPerson;

        cout << "Register new person (yes/no): ";
        getline(cin, choice);

        if (choice != "yes") {
            return;
        }

        cout << "Username: ";
        getline(cin, newPerson.username);

        // Check if the username is already taken
        if (isUsernameTaken(newPerson.username)) {
            cout << "Username already taken. Please choose a different username." << endl;
            return;
        }

        cout << "Password: ";
        getline(cin, newPerson.password);
        cout << "Surname: ";
        getline(cin, newPerson.surname);
        cout << "First Name: ";
        getline(cin, newPerson.firstname);
        cout << "Middle Name: ";
        getline(cin, newPerson.middlename);
        cout << "Date of Birth (MM/DD/YYYY): ";
        getline(cin, newPerson.dateofbirth);
        cout << "Address: ";
        getline(cin, newPerson.address);
        cout << "Civil Status (Single/Married/Divorced/Widowed): ";
        getline(cin, newPerson.civilStatus);
        cout << "Father's Name (Surname, First Name, Middle Initial): ";
        getline(cin, newPerson.fatherFullname);
        cout << "Mother's Maiden Name (Surname, First Name, Middle Initial): ";
        getline(cin, newPerson.motherFullname);
        cout << "Monthly Income Range:" << endl;
        cout << "1. Below PHP 10,000" << endl;
        cout << "2. PHP 10,000 - PHP 20,000" << endl;
        cout << "3. Above PHP 20,000" << endl;
        cout << "Select an option (1/2/3): ";
        int incomeOption;
        cin >> incomeOption;
        cin.ignore(); // Ignore newline left in buffer after cin

        switch (incomeOption) {
            case 1:
                newPerson.income = 10000; // Below PHP 10,000
                break;
            case 2:
                newPerson.income = 15000; // PHP 10,000 - PHP 20,000 (average)
                break;
            case 3:
                newPerson.income = 25000; // Above PHP 20,000
                break;
            default:
                newPerson.income = 0; // Default to 0 if invalid option
                break;
        }

        ofstream writefile(file, ios::app);
        if (!writefile) {
            cerr << "Failed to open file." << endl;
            return;
        }
        writefile << newPerson.username << "|" << newPerson.password << "|" << newPerson.surname << "|" << newPerson.firstname << "|" << newPerson.middlename <<
                  "|" << newPerson.dateofbirth << "|" << newPerson.address << "|" << newPerson.civilStatus <<
                  "|" << newPerson.fatherFullname << "|" << newPerson.motherFullname << "|" << newPerson.income <<
                  endl;

        writefile.close();
        cout << "Account created successfully" << endl;
    }

    void loadperson(string file) {
        ifstream readfile(file);
        if (!readfile) {
            cerr << "Failed to open file." << endl;
            return;
        }

        clearList(); // Clear the current list before loading new data

        string line;
        while (getline(readfile, line)) {
            stringstream ss(line);
            Person person;
            string incomeStr;

            getline(ss, person.username, '|');
            getline(ss, person.password, '|');
            getline(ss, person.surname, '|');
            getline(ss, person.firstname, '|');
            getline(ss, person.middlename, '|');
            getline(ss, person.dateofbirth, '|');
            getline(ss, person.address, '|');
            getline(ss, person.civilStatus, '|');
            getline(ss, person.fatherFullname, '|');
            getline(ss, person.motherFullname, '|');
            getline(ss, incomeStr, '|');

            try {
                person.income = stoi(incomeStr);
            } catch (invalid_argument& e) {
                cerr << "Invalid income value for person: " << person.username << ". Setting income to 0." << endl;
                person.income = 0;
            } catch (out_of_range& e) {
                cerr << "Income value out of range for person: " << person.username << ". Setting income to 0." << endl;
                person.income = 0;
            }

            node* temp = new node;
            temp->person = person;
            temp->next = head;
            head = temp;
        }

        readfile.close();
    }

    bool login(string user, string pass) {
        node* current = head;
        while (current != nullptr) {
            if (current->person.username == user && current->person.password == pass) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    bool adminLogin(const string& user, const string& pass) {
        return user == "admin" && pass == "adminpass";
    }

    void displayList() {
        node* current = head;
        while (current != nullptr) {
            cout << "Username: " << current->person.username << endl;
            cout << "Surname: " << current->person.surname << endl;
            cout << "First Name: " << current->person.firstname << endl;
            cout << "Middle Name: " << current->person.middlename << endl;
            cout << "Date of Birth: " << current->person.dateofbirth << endl;
            cout << "Address: " << current->person.address << endl;
            cout << "Civil Status: " << current->person.civilStatus << endl;
            cout << "Father's Name: " << current->person.fatherFullname << endl;
            cout << "Mother's Maiden Name: " << current->person.motherFullname << endl;
            cout << "Income: " << current->person.income << endl;
            cout << "-----------------------------" << endl;
            current = current->next;
        }
    }

    void displayAccountDetails(string username) {
        node* current = head;
        while (current != nullptr) {
            if (current->person.username == username) {
                cout << "Username: " << current->person.username << endl;
                cout << "Surname: " << current->person.surname << endl;
                cout << "First Name: " << current->person.firstname << endl;
                cout << "Middle Name: " << current->person.middlename << endl;
                cout << "Date of Birth: " << current->person.dateofbirth << endl;
                cout << "Address: " << current->person.address << endl;
                cout << "Civil Status: " << current->person.civilStatus << endl;
                cout << "Father's Name: " << current->person.fatherFullname << endl;
                cout << "Mother's Maiden Name: " << current->person.motherFullname << endl;
                cout << "Income: " << current->person.income << endl;
                cout << "-----------------------------" << endl;
            }
            current = current->next;
        }
    }

    void clearList() {
        node* current = head;
        while (current != nullptr) {
            node* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
    }

    void editPerson(string username) {
        node* current = head;
        while (current != nullptr) {
            if (current->person.username == username) {
                string newValue;

                cout << "Editing details for user: " << username << endl;

                cout << "New Password (leave empty to keep current): ";
                getline(cin, newValue);
                if (!newValue.empty()) current->person.password = newValue;

                cout << "New Surname (leave empty to keep current): ";
                getline(cin, newValue);
                if (!newValue.empty()) current->person.surname = newValue;

                cout << "New First Name (leave empty to keep current): ";
                getline(cin, newValue);
                if (!newValue.empty()) current->person.firstname = newValue;

                cout << "New Middle Name (leave empty to keep current): ";
                getline(cin, newValue);
                if (!newValue.empty()) current->person.middlename = newValue;

                cout << "New Date of Birth (leave empty to keep current): ";
                getline(cin, newValue);
                if (!newValue.empty()) current->person.dateofbirth = newValue;

                cout << "New Address (leave empty to keep current): ";
                getline(cin, newValue);
                if (!newValue.empty()) current->person.address = newValue;

                cout << "New Civil Status (leave empty to keep current): ";
                getline(cin, newValue);
                if (!newValue.empty()) current->person.civilStatus = newValue;

                cout << "New Father's Name (leave empty to keep current): ";
                getline(cin, newValue);
                if (!newValue.empty()) current->person.fatherFullname = newValue;

                cout << "New Mother's Maiden Name (leave empty to keep current): ";
                getline(cin, newValue);
                if (!newValue.empty()) current->person.motherFullname = newValue;

            cout << "New Monthly Income Range:" << endl;
            cout << "1. Below PHP 10,000" << endl;
            cout << "2. PHP 10,000 - PHP 20,000" << endl;
            cout << "3. Above PHP 20,000" << endl;
            cout << "Select an option (1/2/3) or leave empty to keep current: ";
            string incomeOptionStr;
            getline(cin, incomeOptionStr);
            try {
                if (!incomeOptionStr.empty()) {
                    int incomeOption = stoi(incomeOptionStr);
                    switch (incomeOption) {
                        case 1:
                            current->person.income = 10000; // Below PHP 10,000
                            break;
                        case 2:
                            current->person.income = 15000; // PHP 10,000 - PHP 20,000 (average)
                            break;
                        case 3:
                            current->person.income = 25000; // Above PHP 20,000
                            break;
                        default:
                            cout << "Invalid option. Keeping current income." << endl;
                            break;
                    }
                }
            } catch (invalid_argument& e) {
                cout << "Invalid input. Keeping current income." << endl;
            } catch (out_of_range& e) {
                cout << "Input out of range. Keeping current income." << endl;
            }

            saveToFile(global_filename);
            return;
        }
        current = current->next;
    }
    cout << "User not found." << endl;
}

void saveToFile(string file) {
    ofstream writefile(file);
    if (!writefile) {
        cerr << "Failed to open file." << endl;
        return;
    }

    node* current = head;
    while (current != nullptr) {
        writefile << current->person.username << "|" << current->person.password << "|" << current->person.surname << "|" << current->person.firstname << "|" << current->person.middlename <<
                  "|" << current->person.dateofbirth << "|" << current->person.address << "|" << current->person.civilStatus <<
                  "|" << current->person.fatherFullname << "|" << current->person.motherFullname << "|" << current->person.income <<
                  endl;
        current = current->next;
    }

    writefile.close();
}
};
int main() {
    linked_list list;
    list.loadperson(global_filename);

    bool loggedIn = false;
    bool adminLoggedIn = false;
    string loggedInUsername;

    int command;

    while (true) {
        if (!loggedIn && !adminLoggedIn) {
            system("clear"); // Clear the screen for Unix-like systems
            cout << "Civil Registrar System" << endl;
            cout << "1. User Login" << endl;
            cout << "2. Admin Login" << endl;
            cout << "3. Register" << endl;
            cout << "4. Exit" << endl;
            cout << "Enter: ";
            cin >> command;
            cin.ignore(); // Ignore newline left in buffer after cin
            system("clear");
            switch (command) {
                case 1: {
                    string usernameMain, passwordMain;
                    cout << "Username: ";
                    getline(cin, usernameMain);
                    cout << "Password: ";
                    getline(cin, passwordMain);
                    if (list.login(usernameMain, passwordMain)) {
                        loggedIn = true;
                        loggedInUsername = usernameMain;
                        cout << "Login successful!" << endl;
                    } else {
                        cout << "Login failed. Incorrect username or password." << endl;
                    }
                    break;
                }
                case 2: {
                    string adminUsername, adminPassword;
                    cout << "Admin Username: ";
                    getline(cin, adminUsername);
                    cout << "Admin Password: ";
                    getline(cin, adminPassword);
                    if (list.adminLogin(adminUsername, adminPassword)) {
                        adminLoggedIn = true;
                        cout << "Admin login successful!" << endl;
                    } else {
                        cout << "Admin login failed. Incorrect username or password." << endl;
                    }
                    break;
                }
                case 3:
                    list.registerPerson(global_filename);
                    list.loadperson(global_filename);
                    break;
                case 4:
                    cout << "Exiting..." << endl;
                    return 0;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }

            cout << "Press Enter to continue...";
            cin.get(); // Wait for user input
        } else if (adminLoggedIn) {
            system("clear"); // Clear the screen for Unix-like systems
            cout << "Civil Registrar System - Admin" << endl;
            cout << "1. Display All Data" << endl;
            cout << "2. Exit" << endl;
            cout << "Enter: ";
            cin >> command;
            cin.ignore(); // Ignore newline left in buffer after cin
            system("clear");
            switch (command) {
                case 1:
                    list.displayList();
                    break;
                case 2:
                    adminLoggedIn = false;
                    cout << "Logged out successfully." << endl;
                    break;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }

            cout << "Press Enter to continue...";
            cin.get(); // Wait for user input
        } else if (loggedIn) {
            system("clear"); // Clear the screen for Unix-like systems
            cout << "Civil Registrar System - User" << endl;
            cout << "1. Display Personal Information" << endl;
            cout << "2. Edit Person Details" << endl;
            cout << "3. Logout" << endl;
            cout << "Enter: ";
            cin >> command;
            cin.ignore(); // Ignore newline left in buffer after cin
            system("clear");
            switch (command) {
                case 1:
                    list.displayAccountDetails(loggedInUsername);
                    break;
                case 2:
                    list.editPerson(loggedInUsername);
                    break;
                case 3:
                    loggedIn = false;
                    loggedInUsername.clear();
                    cout << "Logged out successfully." << endl;
                    break;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }

            cout << "Press Enter to continue...";
            cin.get(); // Wait for user input
        }
    }

    return 0;
}