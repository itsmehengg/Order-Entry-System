#include "ProductCRUD.h"

// Initialize products vector (will be loaded from file)
vector<Product> products;

// Function to load products from text file
void loadProductsFromFile() {
    ifstream file("products.txt");
    string line;

    products.clear(); // Clear existing products

    if (!file.is_open()) {
        // If file doesn't exist, create default products and save to file
        cout << "Product database file not found. Creating default products..." << endl;
        products = {
            {1, "Laptop", "Electronics", 999.99, 10, "High-performance laptop"},
            {2, "Mouse", "Electronics", 25.50, 50, "Wireless optical mouse"},
            {3, "Keyboard", "Electronics", 75.00, 30, "Mechanical gaming keyboard"},
            {4, "Monitor", "Electronics", 299.99, 15, "24-inch LED monitor"},
            {5, "Headphones", "Electronics", 150.00, 25, "Noise-cancelling headphones"}
        };
        saveProductsToFile();
        cout << "Default products created successfully!" << endl;
        return;
    }

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string idStr, name, category, priceStr, quantityStr, description;

        if (getline(ss, idStr, ',') &&
            getline(ss, name, ',') &&
            getline(ss, category, ',') &&
            getline(ss, priceStr, ',') &&
            getline(ss, quantityStr, ',') &&
            getline(ss, description)) {

            int id = stoi(idStr);
            double price = stod(priceStr);
            int quantity = stoi(quantityStr);

            products.push_back({ id, name, category, price, quantity, description });
        }
    }

    file.close();

    // If file was empty or corrupted, create default products
    if (products.empty()) {
        cout << "Product database file is empty or corrupted. Creating default products..." << endl;
        products = {
            {1, "Laptop", "Electronics", 999.99, 10, "High-performance laptop"},
            {2, "Mouse", "Electronics", 25.50, 50, "Wireless optical mouse"},
            {3, "Keyboard", "Electronics", 75.00, 30, "Mechanical gaming keyboard"}
        };
        saveProductsToFile();
        cout << "Default products created successfully!" << endl;
    }

    cout << "Loaded " << products.size() << " products from database." << endl;
}

// Function to save products to text file
void saveProductsToFile() {
    ofstream file("products.txt");

    if (!file.is_open()) {
        cout << "Error: Unable to save product data to file!" << endl;
        system("pause");
        return;
    }

    for (const auto& product : products) {
        file << product.id << "," << product.name << "," << product.category << ","
            << product.price << "," << product.quantity << "," << product.description << endl;
    }

    file.close();
    cout << "Product data saved successfully!" << endl;
}

// Function to generate unique product ID
int generateProductId() {
    int maxId = 0;
    for (const auto& product : products) {
        if (product.id > maxId) {
            maxId = product.id;
        }
    }
    return maxId + 1;
}

// Function to check if product exists
bool productExists(int id) {
    for (const auto& product : products) {
        if (product.id == id) {
            return true;
        }
    }
    return false;
}

// Function to find a product by ID
Product* findProduct(int id) {
    for (auto& product : products) {
        if (product.id == id) {
            return &product;
        }
    }
    return nullptr;
}

// Function to display product details
void displayProductDetails(const Product& product) {
    cout << "ID: " << product.id << endl;
    cout << "Name: " << product.name << endl;
    cout << "Category: " << product.category << endl;
    cout << "Price: $" << fixed << setprecision(2) << product.price << endl;
    cout << "Quantity: " << product.quantity << endl;
    cout << "Description: " << product.description << endl;
    cout << "----------------------------------------" << endl;
}

// Function to add new product
void addNewProduct() {
    string name, category, description;
    double price;
    int quantity;

    system("cls");
    cout << "========================================" << endl;
    cout << "          ADD NEW PRODUCT" << endl;
    cout << "========================================" << endl;

    cout << "Enter product name: ";
    cin.ignore();
    getline(cin, name);

    cout << "Enter category: ";
    getline(cin, category);

    cout << "Enter price: $";
    cin >> price;

    cout << "Enter quantity: ";
    cin >> quantity;

    cout << "Enter description: ";
    cin.ignore();
    getline(cin, description);

    int newId = generateProductId();
    products.push_back({ newId, name, category, price, quantity, description });
    saveProductsToFile();

    cout << "Product '" << name << "' (ID: " << newId << ") added successfully!" << endl;
    system("pause");
}

// Function to view all products
void viewAllProducts() {
    system("cls");
    cout << "========================================" << endl;
    cout << "           ALL PRODUCTS" << endl;
    cout << "========================================" << endl;

    if (products.empty()) {
        cout << "No products found in database." << endl;
        system("pause");
        return;
    }

    cout << left << setw(5) << "ID" << setw(20) << "Name" << setw(15) << "Category"
        << setw(10) << "Price" << setw(8) << "Qty" << "Description" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;

    for (const auto& product : products) {
        cout << left << setw(5) << product.id
            << setw(20) << product.name
            << setw(15) << product.category
            << setw(10) << fixed << setprecision(2) << product.price
            << setw(8) << product.quantity
            << product.description << endl;
    }

    cout << "========================================" << endl;
    cout << "Total products: " << products.size() << endl;
    system("pause");
}

// Function to view products by category
void viewProductsByCategory() {
    string category;

    system("cls");
    cout << "========================================" << endl;
    cout << "      PRODUCTS BY CATEGORY" << endl;
    cout << "========================================" << endl;

    cout << "Enter category to filter: ";
    cin.ignore();
    getline(cin, category);

    bool found = false;
    cout << "\nProducts in category '" << category << "':" << endl;
    cout << "----------------------------------------" << endl;

    for (const auto& product : products) {
        if (product.category == category) {
            displayProductDetails(product);
            found = true;
        }
    }

    if (!found) {
        cout << "No products found in category '" << category << "'." << endl;
    }

    system("pause");
}

// Function to update product
void updateProduct() {
    int id;

    system("cls");
    cout << "========================================" << endl;
    cout << "         UPDATE PRODUCT" << endl;
    cout << "========================================" << endl;

    cout << "Enter product ID to update: ";
    cin >> id;

    Product* product = findProduct(id);
    if (product == nullptr) {
        cout << "Product not found!" << endl;
        system("pause");
        return;
    }

    cout << "\nCurrent product details:" << endl;
    displayProductDetails(*product);

    cout << "Enter new details (press Enter to keep current value):" << endl;

    string input;
    cin.ignore();

    cout << "Name [" << product->name << "]: ";
    getline(cin, input);
    if (!input.empty()) product->name = input;

    cout << "Category [" << product->category << "]: ";
    getline(cin, input);
    if (!input.empty()) product->category = input;

    cout << "Price [" << product->price << "]: ";
    getline(cin, input);
    if (!input.empty()) product->price = stod(input);

    cout << "Quantity [" << product->quantity << "]: ";
    getline(cin, input);
    if (!input.empty()) product->quantity = stoi(input);

    cout << "Description [" << product->description << "]: ";
    getline(cin, input);
    if (!input.empty()) product->description = input;

    saveProductsToFile();
    cout << "Product updated successfully!" << endl;
    system("pause");
}

// Function to delete product
void deleteProduct() {
    int id;

    system("cls");
    cout << "========================================" << endl;
    cout << "         DELETE PRODUCT" << endl;
    cout << "========================================" << endl;

    cout << "Enter product ID to delete: ";
    cin >> id;

    for (auto it = products.begin(); it != products.end(); ++it) {
        if (it->id == id) {
            cout << "Product to be deleted:" << endl;
            displayProductDetails(*it);

            char confirm;
            cout << "Are you sure you want to delete this product? (y/n): ";
            cin >> confirm;

            if (confirm == 'y' || confirm == 'Y') {
                products.erase(it);
                saveProductsToFile();
                cout << "Product deleted successfully!" << endl;
            }
            else {
                cout << "Delete operation cancelled." << endl;
            }
            system("pause");
            return;
        }
    }

    cout << "Product not found!" << endl;
    system("pause");
}

// Function to search product
void searchProduct() {
    string searchTerm;

    system("cls");
    cout << "========================================" << endl;
    cout << "         SEARCH PRODUCT" << endl;
    cout << "========================================" << endl;

    cout << "Enter product name or ID to search: ";
    cin.ignore();
    getline(cin, searchTerm);

    bool found = false;
    cout << "\nSearch results:" << endl;
    cout << "----------------------------------------" << endl;

    for (const auto& product : products) {
        if (product.name.find(searchTerm) != string::npos ||
            to_string(product.id) == searchTerm) {
            displayProductDetails(product);
            found = true;
        }
    }

    if (!found) {
        cout << "No products found matching '" << searchTerm << "'." << endl;
    }

    system("pause");
}

// Product management menu
int productManagementMenu() {
    int choice;

    do {
        system("cls");
        cout << "========================================" << endl;
        cout << "       PRODUCT MANAGEMENT" << endl;
        cout << "========================================" << endl;
        cout << "1. Add New Product" << endl;
        cout << "2. View All Products" << endl;
        cout << "3. View Products by Category" << endl;
        cout << "4. Update Product" << endl;
        cout << "5. Delete Product" << endl;
        cout << "6. Search Product" << endl;
        cout << "7. Back to Main Menu" << endl;
        cout << "Please select an option (1-7): ";

        cin >> choice;

        if (choice < 1 || choice > 7) {
            cout << "Invalid choice, please try again." << endl;
            system("pause");
        }

    } while (choice < 1 || choice > 7);

    return choice;
}

// Handle product management operations
void handleProductManagement() {
    // Load products when entering product management
    cout << "Loading product database..." << endl;
    loadProductsFromFile();
    system("pause");

    int choice;
    do {
        choice = productManagementMenu();

        switch (choice) {
        case 1:
            addNewProduct();
            break;
        case 2:
            viewAllProducts();
            break;
        case 3:
            viewProductsByCategory();
            break;
        case 4:
            updateProduct();
            break;
        case 5:
            deleteProduct();
            break;
        case 6:
            searchProduct();
            break;
        case 7:
            cout << "Returning to main menu..." << endl;
            system("pause");
            break;
        }
    } while (choice != 7);
}