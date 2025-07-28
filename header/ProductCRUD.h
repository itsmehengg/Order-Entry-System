#pragma once
#ifndef PRODUCTCRUD_H
#define PRODUCTCRUD_H

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <sstream>
using namespace std;

// Structure to store product information
struct Product {
    int id;
    string name;
    string category;
    double price;
    int quantity;
    string description;
};

// Global products vector
extern vector<Product> products;

// Function declarations for Product CRUD operations
void loadProductsFromFile();
void saveProductsToFile();
void addNewProduct();
void viewAllProducts();
void viewProductsByCategory();
void updateProduct();
void deleteProduct();
void searchProduct();
bool productExists(int id);
Product* findProduct(int id);
int generateProductId();
void displayProductDetails(const Product& product);

// Product menu functions
int productManagementMenu();
void handleProductManagement();

#endif