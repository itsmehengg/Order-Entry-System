#pragma once
#ifndef USERCRUD_H
#define USERCRUD_H

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <sstream>
using namespace std;

// Structure to store user credentials
struct User {
    string username;
    string password;
    bool isAdmin;
};

// Global users vector
extern vector<User> users;

// Function declarations for CRUD operations
void loadUsersFromFile();
void saveUsersToFile();
void addNewUser();
void viewAllUsers();
void deleteUser();
bool userExists(const string& username);
User* findUser(const string& username);
void updateUserPassword(const string& username);

#endif