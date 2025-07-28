#include "UserCRUD.h"

// Initialize users vector (will be loaded from file)
vector<User> users;

// Function to load users from text file
void loadUsersFromFile() {
    ifstream file("users.txt");
    string line;

    users.clear(); // Clear existing users

    if (!file.is_open()) {
        // If file doesn't exist, create default users and save to file


        saveUsersToFile();
        cout << "Default users created successfully!" << endl;
        return;
    }

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string username, password, adminStr;

        if (getline(ss, username, ',') &&
            getline(ss, password, ',') &&
            getline(ss, adminStr)) {

            bool isAdmin = (adminStr == "1" || adminStr == "true");
            users.push_back({ username, password, isAdmin });
        }
    }

    file.close();

}

// Function to save users to text file
void saveUsersToFile() {
    ofstream file("users.txt");

    if (!file.is_open()) {
        cout << "Error: Unable to save user data to file!" << endl;
        system("pause");
        return;
    }

    for (const auto& user : users) {
        file << user.username << "," << user.password << "," << (user.isAdmin ? "1" : "0") << endl;
    }

    file.close();
    cout << "User data saved successfully!" << endl;
}

// Function to check if user exists
bool userExists(const string& username) {
    for (const auto& user : users) {
        if (user.username == username) {
            return true;
        }
    }
    return false;
}

// Function to find a user by username
User* findUser(const string& username) {
    for (auto& user : users) {
        if (user.username == username) {
            return &user;
        }
    }
    return nullptr;
}

// Function to add new user
void addNewUser() {
    string username, password;
    char adminChoice;
    bool isAdmin = false;

    system("cls");
    cout << "========================================" << endl;
    cout << "          ADD NEW USER" << endl;
    cout << "========================================" << endl;

    cout << "Enter username: ";
    cin >> username;

    // Check if username already exists
    if (userExists(username)) {
        cout << "Username already exists! Please choose a different username." << endl;
        system("pause");
        return;
    }

    cout << "Enter password: ";
    cin >> password;

    cout << "Is this user an admin? (y/n): ";
    cin >> adminChoice;

    if (adminChoice == 'y' || adminChoice == 'Y') {
        isAdmin = true;
    }

    users.push_back({ username, password, isAdmin });
    saveUsersToFile();

    cout << "User '" << username << "' added successfully!" << endl;
    system("pause");
}

// Function to view all users (admin only)
void viewAllUsers() {
    system("cls");
    cout << "========================================" << endl;
    cout << "           ALL USERS" << endl;
    cout << "========================================" << endl;
    cout << left << setw(15) << "Username" << setw(15) << "Password" << setw(10) << "Type" << endl;
    cout << "----------------------------------------" << endl;

    for (const auto& user : users) {
        cout << left << setw(15) << user.username
            << setw(15) << user.password
            << setw(10) << (user.isAdmin ? "Admin" : "User") << endl;
    }

    cout << "========================================" << endl;
    cout << "Total users: " << users.size() << endl;
    system("pause");
}

// Function to delete user (admin only)
void deleteUser() {
    string username;

    system("cls");
    cout << "========================================" << endl;
    cout << "          DELETE USER" << endl;
    cout << "========================================" << endl;

    cout << "Enter username to delete: ";
    cin >> username;

    // Don't allow deletion of admin user
    if (username == "admin") {
        cout << "Cannot delete the main admin user!" << endl;
        system("pause");
        return;
    }

    for (auto it = users.begin(); it != users.end(); ++it) {
        if (it->username == username) {
            users.erase(it);
            saveUsersToFile();
            cout << "User '" << username << "' deleted successfully!" << endl;
            system("pause");
            return;
        }
    }

    cout << "User not found!" << endl;
    system("pause");
}

// Function to update user password
void updateUserPassword(const string& username) {
    User* user = findUser(username);

    if (user == nullptr) {
        cout << "User not found!" << endl;
        system("pause");
        return;
    }

    string newPassword;
    system("cls");
    cout << "========================================" << endl;
    cout << "        UPDATE PASSWORD" << endl;
    cout << "========================================" << endl;
    cout << "Username: " << username << endl;
    cout << "Current Password: " << user->password << endl;
    cout << "Enter new password: ";
    cin >> newPassword;

    user->password = newPassword;
    saveUsersToFile();

    cout << "Password updated successfully!" << endl;
    system("pause");
}