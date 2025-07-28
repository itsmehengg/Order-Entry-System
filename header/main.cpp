#pragma once
#include "UserCRUD.h"
#include "ProductCRUD.h"
#include "PaymentCRUD.h"

int userLogin() {
    // Load users from file at the beginning
    cout << "Initializing user database..." << endl;
    loadUsersFromFile();
    system("pause");

    string username, password;
    int attempts = 0;
    const int maxAttempts = 3;

    do {
        system("cls");
        cout << "========================================" << endl;
        cout << "           LOGIN SYSTEM" << endl;
        cout << "========================================" << endl;
        cout << endl;

        if (attempts > 0) {
            cout << "Invalid credentials! Attempts remaining: " << (maxAttempts - attempts) << endl;
            cout << endl;
        }

        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;

        // Check credentials
        for (const auto& user : users) {
            if (user.username == username && user.password == password) {
                system("cls");
                cout << "Login successful!" << endl;
                cout << "Welcome, " << username << "!" << endl;

                if (user.isAdmin) {
                    cout << "Access Level: ADMINISTRATOR" << endl;
                    system("pause");
                    return 1; // Admin mode
                }
                else {
                    cout << "Access Level: USER" << endl;
                    system("pause");
                    return 2; // User mode
                }
            }
        }

        attempts++;

        if (attempts < maxAttempts) {
            system("pause");
        }

    } while (attempts < maxAttempts);

    // Max attempts reached
    system("cls");
    cout << "Maximum login attempts exceeded!" << endl;
    cout << "Access denied. Exiting system..." << endl;
    system("pause");
    return 0; // Login failed
}

int adminMenu() {
    int choice;

    do {
        system("cls");
        cout << "========================================" << endl;
        cout << "          ADMIN MENU" << endl;
        cout << "========================================" << endl;
        cout << "1. Product Management" << endl;
        cout << "2. Transaction Management" << endl;
        cout << "3. Add New User" << endl;
        cout << "4. View All Users" << endl;
        cout << "5. Delete User" << endl;
        cout << "6. Update User Password" << endl;
        cout << "7. System Settings" << endl;
        cout << "8. View Reports" << endl;
        cout << "9. Logout" << endl;
        cout << "Please select an option (1-9): ";

        cin >> choice;

        if (choice < 1 || choice > 9) {
            cout << "Invalid choice, please try again." << endl;
            system("pause");
        }

    } while (choice < 1 || choice > 9);

    switch (choice) {
    case 1:
        handleProductManagement();
        break;
    case 2:
        handleAdminTransactions();
        break;
    case 3:
        addNewUser();
        break;
    case 4:
        viewAllUsers();
        break;
    case 5:
        deleteUser();
        break;
    case 6:
    {
        string username;
        cout << "Enter username to update password: ";
        cin >> username;
        updateUserPassword(username);
    }
    break;
    case 7:
        cout << "System Settings selected!" << endl;
        system("pause");
        break;
    case 8:
        cout << "View Reports selected!" << endl;
        system("pause");
        break;
    case 9:
        cout << "Logging out..." << endl;
        system("pause");
        break;
    }

    return choice;
}

int userMenu() {
    int choice;

    do {
        system("cls");
        cout << "========================================" << endl;
        cout << "           USER MENU" << endl;
        cout << "========================================" << endl;
        cout << "1. View Products" << endl;
        cout << "2. Shopping & Payment" << endl;
        cout << "3. Account Settings" << endl;
        cout << "4. Logout" << endl;
        cout << "Please select an option (1-4): ";

        cin >> choice;

        if (choice < 1 || choice > 4) {
            cout << "Invalid choice, please try again." << endl;
            system("pause");
        }

    } while (choice < 1 || choice > 4);

    switch (choice) {
    case 1:
        viewAllProducts();
        break;
    case 2:
    {
        // Get current username from login context
        string currentUser = "";
        for (const auto& user : users) {
            // In a real implementation, you'd track the logged-in user
            // For now, we'll ask for username
        }
        cout << "Enter your username: ";
        cin >> currentUser;
        handleUserPayment(currentUser);
    }
    break;
    case 3:
        cout << "Account Settings selected!" << endl;
        system("pause");
        break;
    case 4:
        cout << "Logging out..." << endl;
        system("pause");
        break;
    }

    return choice;
}

int main() {
    int loginResult;
    int menuChoice;

    do {
        loginResult = userLogin();

        if (loginResult == 0) {
            // Login failed, exit program
            return 0;
        }

        do {
            if (loginResult == 1) {
                // Admin mode
                menuChoice = adminMenu();
            }
            else if (loginResult == 2) {
                // User mode
                menuChoice = userMenu();
            }
        } while ((loginResult == 1 && menuChoice != 9) || (loginResult == 2 && menuChoice != 4));

        // User logged out, return to login screen

    } while (true);

    return 0;
}