#ifndef PAYMENTCRUD_H
#define PAYMENTCRUD_H

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <random>
#include "ProductCRUD.h"
using namespace std;

// Structure for transaction items
struct TransactionItem {
    int productId;
    string productName;
    double unitPrice;
    int quantity;
    double totalPrice;
};

// Structure for complete transactions
struct Transaction {
    int transactionId;
    string username;
    string date;
    string time;
    vector<TransactionItem> items;
    double subtotal;
    double tax;
    double totalAmount;
    string paymentMethod;
    string status;
};

// Global transactions vector
extern vector<Transaction> transactions;

// Function declarations for Payment CRUD operations
void loadTransactionsFromFile();
void saveTransactionsToFile();
int generateTransactionId();
string generateCurrentDate();
string generateCurrentTime();

// Shopping cart functions
void addToCart(const string& username);
void viewCart(const string& username);
void removeFromCart(const string& username);
void clearCart(const string& username);
bool processPayment(const string& username);

// Transaction management
void viewUserTransactions(const string& username);
void viewAllTransactions(); // Admin only
void generateInvoice(int transactionId);
void viewSalesReport(); // Admin only

// Payment menu functions
int userPaymentMenu();
int adminTransactionMenu();
void handleUserPayment(const string& username);
void handleAdminTransactions();

// Utility functions
double calculateTax(double subtotal);
bool isProductAvailable(int productId, int requestedQuantity);
void updateProductStock(int productId, int purchasedQuantity);
vector<TransactionItem>* getUserCart(const string& username);

#endif