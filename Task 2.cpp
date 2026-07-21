/*
CodeAlpha Internship - Task 2
Login and Registration System

    Idea:
    - All usernames and passwords are stored in one text file "users.txt"
    - Each line in the file looks like:  username password
    - registerUser() checks if the username already exists before adding it
    - loginUser() reads the file and checks if username+password match
*/

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const string FILE_NAME = "users.txt";

//checks if a username already exists in the file
bool usernameExists(string username) 
{
    ifstream file(FILE_NAME);
    
    //return false immediately if file doesn't exist yet
    if (!file.is_open()) {
        return false;
    }

    string storedUser, storedPass;

    while (file >> storedUser >> storedPass) 
    {
        if (storedUser == username) 
        {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

//registers a new user by appending their details to the file
void registerUser() {
    string username, password;

    cout << "--- REGISTER ---" << endl;
    cout << "Choose a username: ";
    cin >> username;

    if (usernameExists(username)) 
    {
        cout << "Error: username already taken. Try logging in instead." << endl;
        return;
    }

    cout << "Choose a password: ";
    cin >> password;

    //appending (not overwriting) so that the previous users stay in the file
    ofstream file(FILE_NAME, ios::app);
    
    //check if file opened properly for writing
    if (!file.is_open()) {
        cout << "Error: Could not open file for writing." << endl;
        return;
    }

    file << username << " " << password << endl;
    file.flush(); //forces buffer to write file to disk immediately
    file.close();

    cout << "Registration successful! You can now log in." << endl;
}

//logs a user in by checking their credentials against the file
void loginUser() {
    string username, password;

    cout << "--- LOGIN ---" << endl;
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    ifstream file(FILE_NAME);

    //check if users file exists before attempting to read
    if (!file.is_open()) {
        cout << "Login failed: No registered users found." << endl;
        return;
    }

    string storedUser, storedPass;
    bool found = false;

    while (file >> storedUser >> storedPass) {
        if (storedUser == username && storedPass == password) 
        {
            found = true;
            break;
        }
    }
    file.close();

    if (found) {
        cout << "Login successful! Welcome, " << username << "." << endl;
    } else {
        cout << "Login failed: incorrect username or password." << endl;
    }
}

int main() {
    int choice;

    do {
        cout << "===== LOGIN & REGISTRATION SYSTEM =====" << endl;
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                loginUser();
                break;
            case 3:
                cout << "Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice, try again." << endl;
        }

    } while (choice != 3);

    return 0;
}