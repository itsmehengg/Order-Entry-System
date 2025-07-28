#include "PaymentCRUD.h"

// Initialize transactions vector
vector<Transaction> transactions;

// Temporary shopping cart storage (username -> cart items)
vector<pair<string, vector<TransactionItem>>> shoppingCarts;

// Function to load transactions from file
void loadTransactionsFromFile() {
    ifstream file("transactions.txt");
    string line;

    transactions.clear();

    if (!file.is_open()) {
        cout << "Transaction database file not found. Creating new file..." << endl;
        return;
    }

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string transactionIdStr, username, date, time, subtotalStr, taxStr, totalStr, paymentMethod, status;

        if (getline(ss, transactionIdStr, '|') &&
            getline(ss, username, '|') &&
            getline(ss, date, '|') &&
            getline(ss, time, '|') &&
            getline(ss, subtotalStr, '|') &&
            getline(ss, taxStr, '|') &&
            getline(ss, totalStr, '|') &&
            getline(ss, paymentMethod, '|') &&
            getline(ss, status)) {

            Transaction trans;
            trans.transactionId = stoi(transactionIdStr);
            trans.username = username;
            trans.date = date;
            trans.time = time;
            trans.subtotal = stod(subtotalStr);
            trans.tax = stod(taxStr);
            trans.totalAmount = stod(totalStr);
            trans.paymentMethod = paymentMethod;
            trans.status = status;

            transactions.push_back(trans);
        }
    }

    file.close();
    cout << "Loaded " << transactions.size() << " transactions from database." << endl;
}

// Function to save transactions to file
void saveTransactionsToFile() {
    ofstream file("transactions.txt");

    if (!file.is_open()) {
        cout << "Error: Unable to save transaction data to file!" << endl;
        return;
    }

    for (const auto& trans : transactions) {
        file << trans.transactionId << "|" << trans.username << "|" << trans.date << "|"
            << trans.time << "|" << trans.subtotal << "|" << trans.tax << "|"
            << trans.totalAmount << "|" << trans.paymentMethod << "|" << trans.status << endl;
    }

    file.close();
}

// Generate unique transaction ID
int generateTransactionId() {
    int maxId = 0;
    for (const auto& trans : transactions) {
        if (trans.transactionId > maxId) {
            maxId = trans.transactionId;
        }
    }
    return maxId + 1;
}

// Generate current date (simple format)
string generateCurrentDate() {
    // Using a simple counter-based date system for demo
    // In real implementation, you could use a more sophisticated method
    static int dayCounter = 1;

    int day = ((dayCounter % 30) + 1);
    int month = ((dayCounter / 30) % 12) + 1;
    int year = 2025;

    dayCounter++;

    stringstream ss;
    ss << year << "-" << setfill('0') << setw(2) << month << "-" << setfill('0') << setw(2) << day;
    return ss.str();
}

// Generate current time (simple format)
string generateCurrentTime() {
    // Using a simple random time generator for demo
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> hourDist(8, 22);   // Business hours 8-22
    uniform_int_distribution<> minDist(0, 59);
    uniform_int_distribution<> secDist(0, 59);

    int hour = hourDist(gen);
    int minute = minDist(gen);
    int second = secDist(gen);

    stringstream ss;
    ss << setfill('0') << setw(2) << hour << ":"
        << setfill('0') << setw(2) << minute << ":"
        << setfill('0') << setw(2) << second;
    return ss.str();
}

// Calculate tax (10% tax rate)
double calculateTax(double subtotal) {
    return subtotal * 0.10;
}

// Check if product is available in requested quantity
bool isProductAvailable(int productId, int requestedQuantity) {
    Product* product = findProduct(productId);
    if (product == nullptr) return false;
    return product->quantity >= requestedQuantity;
}

// Update product stock after purchase
void updateProductStock(int productId, int purchasedQuantity) {
    Product* product = findProduct(productId);
    if (product != nullptr) {
        product->quantity -= purchasedQuantity;
        saveProductsToFile();
    }
}

// Get user's shopping cart
vector<TransactionItem>* getUserCart(const string& username) {
    for (auto& cart : shoppingCarts) {
        if (cart.first == username) {
            return &cart.second;
        }
    }
    // Create new cart if doesn't exist
    shoppingCarts.push_back({ username, vector<TransactionItem>() });
    return &shoppingCarts.back().second;
}

// Add product to cart
void addToCart(const string& username) {
    int productId, quantity;

    system("cls");
    cout << "========================================" << endl;
    cout << "          ADD TO CART" << endl;
    cout << "========================================" << endl;

    // Show available products first
    viewAllProducts();

    cout << "Enter product ID to add to cart: ";
    cin >> productId;

    Product* product = findProduct(productId);
    if (product == nullptr) {
        cout << "Product not found!" << endl;
        system("pause");
        return;
    }

    if (product->quantity == 0) {
        cout << "========================================" << endl;
        cout << "         PRODUCT UNAVAILABLE" << endl;
        cout << "========================================" << endl;
        cout << "Sorry! Product '" << product->name << "' is" << endl;
        cout << "*** OUT OF STOCK ***" << endl;
        cout << "and UNAVAILABLE FOR PURCHASE." << endl;
        cout << "Please check back later or contact admin." << endl;
        cout << "========================================" << endl;
        system("pause");
        return;
    }

    cout << "Product: " << product->name << endl;
    cout << "Price: $" << fixed << setprecision(2) << product->price << endl;
    cout << "Available quantity: " << product->quantity << endl;
    cout << "Enter quantity to purchase: ";
    cin >> quantity;

    if (quantity <= 0) {
        cout << "Invalid quantity!" << endl;
        system("pause");
        return;
    }

    if (!isProductAvailable(productId, quantity)) {
        cout << "========================================" << endl;
        cout << "    INSUFFICIENT STOCK AVAILABLE" << endl;
        cout << "========================================" << endl;
        cout << "Sorry! Product '" << product->name << "' is" << endl;
        cout << "UNAVAILABLE for the requested quantity." << endl;
        cout << "Available: " << product->quantity << " units" << endl;
        cout << "Requested: " << quantity << " units" << endl;
        cout << "Please reduce quantity or try again later." << endl;
        cout << "========================================" << endl;
        system("pause");
        return;
    }

    vector<TransactionItem>* cart = getUserCart(username);

    // Check if product already in cart
    bool found = false;
    for (auto& item : *cart) {
        if (item.productId == productId) {
            if (isProductAvailable(productId, item.quantity + quantity)) {
                item.quantity += quantity;
                item.totalPrice = item.quantity * item.unitPrice;
                found = true;
                cout << "Updated quantity in cart successfully!" << endl;
            }
            else {
                cout << "Cannot add more - insufficient stock!" << endl;
            }
            break;
        }
    }

    if (!found) {
        TransactionItem item;
        item.productId = productId;
        item.productName = product->name;
        item.unitPrice = product->price;
        item.quantity = quantity;
        item.totalPrice = quantity * product->price;
        cart->push_back(item);
        cout << "Product added to cart successfully!" << endl;
    }

    system("pause");
}

// View shopping cart
void viewCart(const string& username) {
    system("cls");
    cout << "========================================" << endl;
    cout << "         SHOPPING CART" << endl;
    cout << "         User: " << username << endl;
    cout << "========================================" << endl;

    vector<TransactionItem>* cart = getUserCart(username);

    if (cart->empty()) {
        cout << "Your cart is empty!" << endl;
        cout << "Add some products to start shopping." << endl;
        system("pause");
        return;
    }

    cout << left << setw(5) << "ID" << setw(20) << "Product" << setw(10) << "Price"
        << setw(8) << "Qty" << setw(12) << "Total" << endl;
    cout << "-------------------------------------------------------" << endl;

    double cartTotal = 0;
    for (const auto& item : *cart) {
        cout << left << setw(5) << item.productId
            << setw(20) << item.productName
            << setw(10) << fixed << setprecision(2) << item.unitPrice
            << setw(8) << item.quantity
            << setw(12) << item.totalPrice << endl;
        cartTotal += item.totalPrice;
    }

    cout << "-------------------------------------------------------" << endl;
    cout << "Cart Subtotal: $" << fixed << setprecision(2) << cartTotal << endl;
    cout << "Tax (10%): $" << calculateTax(cartTotal) << endl;
    cout << "========================================" << endl;
    cout << "TOTAL AMOUNT: $" << cartTotal + calculateTax(cartTotal) << endl;
    cout << "========================================" << endl;
    system("pause");
}

// Remove item from cart
void removeFromCart(const string& username) {
    system("cls");
    cout << "========================================" << endl;
    cout << "       REMOVE FROM CART" << endl;
    cout << "========================================" << endl;

    vector<TransactionItem>* cart = getUserCart(username);

    if (cart->empty()) {
        cout << "Your cart is empty!" << endl;
        system("pause");
        return;
    }

    // Show current cart
    cout << "Current cart items:" << endl;
    cout << left << setw(5) << "ID" << setw(20) << "Product" << setw(8) << "Qty" << endl;
    cout << "---------------------------------------" << endl;
    for (const auto& item : *cart) {
        cout << left << setw(5) << item.productId
            << setw(20) << item.productName
            << setw(8) << item.quantity << endl;
    }
    cout << "---------------------------------------" << endl;

    int productId;
    cout << "Enter product ID to remove: ";
    cin >> productId;

    for (auto it = cart->begin(); it != cart->end(); ++it) {
        if (it->productId == productId) {
            cout << "Removed '" << it->productName << "' from cart." << endl;
            cart->erase(it);
            system("pause");
            return;
        }
    }

    cout << "Product not found in cart!" << endl;
    system("pause");
}

// Clear entire cart
void clearCart(const string& username) {
    vector<TransactionItem>* cart = getUserCart(username);
    cart->clear();
    cout << "Cart cleared successfully!" << endl;
}

// Process payment and complete transaction
bool processPayment(const string& username) {
    vector<TransactionItem>* cart = getUserCart(username);

    if (cart->empty()) {
        cout << "Your cart is empty! Add items before checkout." << endl;
        system("pause");
        return false;
    }

    system("cls");
    cout << "========================================" << endl;
    cout << "           CHECKOUT" << endl;
    cout << "========================================" << endl;

    // Display cart summary
    double subtotal = 0;
    cout << "Order Summary:" << endl;
    cout << left << setw(20) << "Product" << setw(8) << "Qty" << setw(12) << "Total" << endl;
    cout << "----------------------------------------" << endl;

    for (const auto& item : *cart) {
        cout << left << setw(20) << item.productName
            << setw(8) << item.quantity
            << setw(12) << fixed << setprecision(2) << item.totalPrice << endl;
        subtotal += item.totalPrice;
    }

    double tax = calculateTax(subtotal);
    double total = subtotal + tax;

    cout << "----------------------------------------" << endl;
    cout << "Subtotal: $" << fixed << setprecision(2) << subtotal << endl;
    cout << "Tax (10%): $" << tax << endl;
    cout << "========================================" << endl;
    cout << "TOTAL: $" << total << endl;
    cout << "========================================" << endl;

    // Payment method selection
    cout << "Select Payment Method:" << endl;
    cout << "1. Credit Card" << endl;
    cout << "2. Debit Card" << endl;
    cout << "3. Cash" << endl;
    cout << "4. Cancel" << endl;
    cout << "Choice: ";

    int paymentChoice;
    cin >> paymentChoice;

    if (paymentChoice == 4) {
        cout << "Payment cancelled." << endl;
        system("pause");
        return false;
    }

    string paymentMethods[] = { "", "Credit Card", "Debit Card", "Cash" };
    if (paymentChoice < 1 || paymentChoice > 3) {
        cout << "Invalid payment method!" << endl;
        system("pause");
        return false;
    }

    // Final confirmation
    char confirm;
    cout << "Confirm payment of $" << total << " using " << paymentMethods[paymentChoice] << "? (y/n): ";
    cin >> confirm;

    if (confirm != 'y' && confirm != 'Y') {
        cout << "Payment cancelled." << endl;
        system("pause");
        return false;
    }

    // Check stock availability again before processing
    for (const auto& item : *cart) {
        if (!isProductAvailable(item.productId, item.quantity)) {
            Product* product = findProduct(item.productId);
            cout << "========================================" << endl;
            cout << "     STOCK UNAVAILABLE" << endl;
            cout << "========================================" << endl;
            cout << "Sorry! Product '" << item.productName << "' is no longer" << endl;
            cout << "available in the requested quantity." << endl;
            cout << "Available: " << (product ? product->quantity : 0) << endl;
            cout << "Requested: " << item.quantity << endl;
            cout << "Please update your cart and try again." << endl;
            cout << "========================================" << endl;
            system("pause");
            return false;
        }
    }

    // Create transaction record
    Transaction transaction;
    transaction.transactionId = generateTransactionId();
    transaction.username = username;
    transaction.date = generateCurrentDate();
    transaction.time = generateCurrentTime();
    transaction.items = *cart;
    transaction.subtotal = subtotal;
    transaction.tax = tax;
    transaction.totalAmount = total;
    transaction.paymentMethod = paymentMethods[paymentChoice];
    transaction.status = "Completed";

    // Update product stock
    for (const auto& item : *cart) {
        updateProductStock(item.productId, item.quantity);
    }

    // Save transaction
    transactions.push_back(transaction);
    saveTransactionsToFile();

    // Generate invoice
    generateInvoice(transaction.transactionId);

    // Clear cart
    clearCart(username);

    cout << "========================================" << endl;
    cout << "       PAYMENT SUCCESSFUL!" << endl;
    cout << "========================================" << endl;
    cout << "Transaction ID: " << transaction.transactionId << endl;
    cout << "Amount Paid: $" << fixed << setprecision(2) << total << endl;
    cout << "Payment Method: " << transaction.paymentMethod << endl;
    cout << "Date: " << transaction.date << endl;
    cout << "Time: " << transaction.time << endl;
    cout << "Invoice generated successfully!" << endl;
    cout << "========================================" << endl;
    system("pause");
    return true;
}

// Generate invoice
void generateInvoice(int transactionId) {
    Transaction* trans = nullptr;
    for (auto& t : transactions) {
        if (t.transactionId == transactionId) {
            trans = &t;
            break;
        }
    }

    if (trans == nullptr) {
        cout << "Transaction not found!" << endl;
        return;
    }

    // Create invoice file
    string filename = "invoice_" + to_string(transactionId) + ".txt";
    ofstream invoiceFile(filename);

    if (!invoiceFile.is_open()) {
        cout << "Error creating invoice file!" << endl;
        return;
    }

    // Invoice content
    invoiceFile << "========================================" << endl;
    invoiceFile << "           SALES INVOICE" << endl;
    invoiceFile << "      Electronic Store System" << endl;
    invoiceFile << "========================================" << endl;
    invoiceFile << "Invoice #: " << trans->transactionId << endl;
    invoiceFile << "Customer: " << trans->username << endl;
    invoiceFile << "Date: " << trans->date << endl;
    invoiceFile << "Time: " << trans->time << endl;
    invoiceFile << "Payment Method: " << trans->paymentMethod << endl;
    invoiceFile << "========================================" << endl;
    invoiceFile << endl;

    invoiceFile << "ITEMS PURCHASED:" << endl;
    invoiceFile << left << setw(20) << "Product" << setw(10) << "Price"
        << setw(8) << "Qty" << setw(12) << "Total" << endl;
    invoiceFile << "-------------------------------------------------------" << endl;

    for (const auto& item : trans->items) {
        invoiceFile << left << setw(20) << item.productName
            << setw(10) << fixed << setprecision(2) << item.unitPrice
            << setw(8) << item.quantity
            << setw(12) << item.totalPrice << endl;
    }

    invoiceFile << "-------------------------------------------------------" << endl;
    invoiceFile << "Subtotal: $" << fixed << setprecision(2) << trans->subtotal << endl;
    invoiceFile << "Tax (10%): $" << trans->tax << endl;
    invoiceFile << "========================================" << endl;
    invoiceFile << "TOTAL PAID: $" << trans->totalAmount << endl;
    invoiceFile << "========================================" << endl;
    invoiceFile << "Status: " << trans->status << endl;
    invoiceFile << endl;
    invoiceFile << "Thank you for your purchase!" << endl;
    invoiceFile << "Visit us again soon!" << endl;

    invoiceFile.close();
}

// View user's transaction history
void viewUserTransactions(const string& username) {
    system("cls");
    cout << "========================================" << endl;
    cout << "       TRANSACTION HISTORY" << endl;
    cout << "       User: " << username << endl;
    cout << "========================================" << endl;

    bool found = false;
    for (const auto& trans : transactions) {
        if (trans.username == username) {
            cout << "Transaction ID: " << trans.transactionId << endl;
            cout << "Date: " << trans.date << " " << trans.time << endl;
            cout << "Total: $" << fixed << setprecision(2) << trans.totalAmount << endl;
            cout << "Payment: " << trans.paymentMethod << endl;
            cout << "Status: " << trans.status << endl;
            cout << "Items: " << trans.items.size() << " products" << endl;
            cout << "----------------------------------------" << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "No transactions found." << endl;
        cout << "Start shopping to see your purchase history!" << endl;
    }

    system("pause");
}

// View all transactions (admin only)
void viewAllTransactions() {
    system("cls");
    cout << "========================================" << endl;
    cout << "       ALL TRANSACTIONS (ADMIN)" << endl;
    cout << "========================================" << endl;

    if (transactions.empty()) {
        cout << "No transactions found in the system." << endl;
        system("pause");
        return;
    }

    cout << left << setw(8) << "ID" << setw(15) << "Customer" << setw(12) << "Date"
        << setw(10) << "Total" << setw(15) << "Payment" << "Status" << endl;
    cout << "------------------------------------------------------------------------" << endl;

    double totalRevenue = 0;
    for (const auto& trans : transactions) {
        cout << left << setw(8) << trans.transactionId
            << setw(15) << trans.username
            << setw(12) << trans.date
            << setw(10) << fixed << setprecision(2) << trans.totalAmount
            << setw(15) << trans.paymentMethod
            << trans.status << endl;
        totalRevenue += trans.totalAmount;
    }

    cout << "------------------------------------------------------------------------" << endl;
    cout << "Total transactions: " << transactions.size() << endl;
    cout << "Total revenue: $" << fixed << setprecision(2) << totalRevenue << endl;
    cout << "========================================" << endl;
    system("pause");
}

// Generate sales report (admin only)
void viewSalesReport() {
    system("cls");
    cout << "========================================" << endl;
    cout << "         SALES REPORT (ADMIN)" << endl;
    cout << "========================================" << endl;

    if (transactions.empty()) {
        cout << "No sales data available." << endl;
        system("pause");
        return;
    }

    double totalRevenue = 0;
    double totalTax = 0;
    int totalTransactions = transactions.size();

    for (const auto& trans : transactions) {
        totalRevenue += trans.totalAmount;
        totalTax += trans.tax;
    }

    cout << "BUSINESS PERFORMANCE SUMMARY:" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Total Transactions: " << totalTransactions << endl;
    cout << "Total Revenue: $" << fixed << setprecision(2) << totalRevenue << endl;
    cout << "Total Tax Collected: $" << totalTax << endl;
    cout << "Average Transaction: $" << (totalTransactions > 0 ? totalRevenue / totalTransactions : 0) << endl;
    cout << "Net Revenue (after tax): $" << (totalRevenue - totalTax) << endl;
    cout << "========================================" << endl;

    // Payment method breakdown
    int creditCard = 0, debitCard = 0, cash = 0;
    for (const auto& trans : transactions) {
        if (trans.paymentMethod == "Credit Card") creditCard++;
        else if (trans.paymentMethod == "Debit Card") debitCard++;
        else if (trans.paymentMethod == "Cash") cash++;
    }

    cout << "PAYMENT METHOD BREAKDOWN:" << endl;
    cout << "Credit Card: " << creditCard << " transactions" << endl;
    cout << "Debit Card: " << debitCard << " transactions" << endl;
    cout << "Cash: " << cash << " transactions" << endl;
    cout << "========================================" << endl;

    system("pause");
}

// User payment menu
int userPaymentMenu() {
    int choice;

    do {
        system("cls");
        cout << "========================================" << endl;
        cout << "       SHOPPING & PAYMENT" << endl;
        cout << "========================================" << endl;
        cout << "1. Browse Products" << endl;
        cout << "2. Add to Cart" << endl;
        cout << "3. View Cart" << endl;
        cout << "4. Remove from Cart" << endl;
        cout << "5. Checkout & Pay" << endl;
        cout << "6. My Transaction History" << endl;
        cout << "7. Back to Main Menu" << endl;
        cout << "========================================" << endl;
        cout << "Please select an option (1-7): ";

        cin >> choice;

        if (choice < 1 || choice > 7) {
            cout << "Invalid choice, please try again." << endl;
            system("pause");
        }

    } while (choice < 1 || choice > 7);

    return choice;
}

// Admin transaction menu
int adminTransactionMenu() {
    int choice;

    do {
        system("cls");
        cout << "========================================" << endl;
        cout << "     TRANSACTION MANAGEMENT (ADMIN)" << endl;
        cout << "========================================" << endl;
        cout << "1. View All Transactions" << endl;
        cout << "2. Generate Invoice" << endl;
        cout << "3. Sales Report" << endl;
        cout << "4. Back to Main Menu" << endl;
        cout << "========================================" << endl;
        cout << "Please select an option (1-4): ";

        cin >> choice;

        if (choice < 1 || choice > 4) {
            cout << "Invalid choice, please try again." << endl;
            system("pause");
        }

    } while (choice < 1 || choice > 4);

    return choice;
}

// Handle user payment operations
void handleUserPayment(const string& username) {
    // Load transactions and products
    cout << "Loading shopping system..." << endl;
    loadTransactionsFromFile();
    loadProductsFromFile();
    system("pause");

    int choice;
    do {
        choice = userPaymentMenu();

        switch (choice) {
        case 1:
            viewAllProducts();
            break;
        case 2:
            addToCart(username);
            break;
        case 3:
            viewCart(username);
            break;
        case 4:
            removeFromCart(username);
            break;
        case 5:
            processPayment(username);
            break;
        case 6:
            viewUserTransactions(username);
            break;
        case 7:
            cout << "Returning to main menu..." << endl;
            system("pause");
            break;
        }
    } while (choice != 7);
}

// Handle admin transaction operations
void handleAdminTransactions() {
    cout << "Loading transaction management system..." << endl;
    loadTransactionsFromFile();
    system("pause");

    int choice;
    do {
        choice = adminTransactionMenu();

        switch (choice) {
        case 1:
            viewAllTransactions();
            break;
        case 2:
        {
            int transId;
            cout << "Enter transaction ID to generate invoice: ";
            cin >> transId;
            generateInvoice(transId);
            cout << "Invoice generation completed!" << endl;
            system("pause");
        }
        break;
        case 3:
            viewSalesReport();
            break;
        case 4:
            cout << "Returning to main menu..." << endl;
            system("pause");
            break;
        }
    } while (choice != 4);
}