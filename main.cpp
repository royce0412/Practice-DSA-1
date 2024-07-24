#include <iostream>
#include <fstream>
#include <string>
using namespace std;

string global_filename = "details.txt";

struct Person {
    string username;
    string password;
    string fullname;
    string dateofbirth;
    string address;
    string civilStatus;
    string fatherFullname;
    string motherFullname;
    int income; // in PHP (Philippine Peso)
    int catalogNumber; // Catalog number for sorting by surname
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
        cout << "Password: ";
        getline(cin, newPerson.password);
        cout << "Name (Surname, First Name, Middle Initial): ";
        getline(cin, newPerson.fullname);
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
        writefile << newPerson.username << " " << newPerson.password << " " << newPerson.fullname <<
                  " " << newPerson.dateofbirth << " " << newPerson.address << " " << newPerson.civilStatus <<
                  " " << newPerson.fatherFullname << " " << newPerson.motherFullname << " " << newPerson.income <<
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

        Person person;
        while (readfile >> person.username >> person.password >> person.fullname >> person.dateofbirth
                        >> person.address >> person.civilStatus >> person.fatherFullname >> person.motherFullname >>
                        person.income) {
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
};

int main() {
    linked_list list;

    list.loadperson(global_filename);
    int command;

    while (true) {
        cout << "Civil Registrar System" << endl;
        cout << "1. Login" << endl;
        cout << "2. Register" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter: ";
        cin >> command;
        cin.ignore(); // Ignore newline left in buffer after cin

        switch (command) {
            case 1: {
                string usernameMain, passwordMain;
                cout << "Username: ";
                getline(cin, usernameMain);
                cout << "Password: ";
                getline(cin, passwordMain);
                if (list.login(usernameMain, passwordMain)) {
                    cout << "Login successful!" << endl;
                } else {
                    cout << "Login failed. Incorrect username or password." << endl;
                }
                break;
            }
            case 2:
                list.registerPerson(global_filename);
                break;
            case 3:
                cout << "Exiting..." << endl;
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
