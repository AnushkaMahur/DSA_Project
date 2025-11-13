
#include "cart.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <direct.h>   
using namespace std;

//Get the file path where cart data will be stored
string getCartFilePath() {
    char buffer[1024];
    _getcwd(buffer, sizeof(buffer));
    string currentPath(buffer);

    
    size_t pos = currentPath.find("gui_python");
    if (pos != string::npos) {
        currentPath = currentPath.substr(0, pos) + "backend_cpp";
    }

    return currentPath + "\\cart_data.txt";
}

//Adding an item to the cart with a specific quantity
bool ShoppingCart::addItem(Product* product, int quantity) {
    if (!product || quantity <= 0)
        return false;

    //if requested quantity exists
    if (product->stock < quantity) {
        cout << "ERROR: Insufficient stock for " << product->name << endl;
        return false;
    }

    //if product already exists in cart
    for (auto& item : items) {
        if (item.product->name == product->name) {
            if (product->stock < item.quantity + quantity) {    //checking availability in the stock
                cout << "ERROR: Insufficient stock" << endl;
                return false;
            }
            //Update existing quantity
            item.quantity += quantity;
            cout << "SUCCESS: Updated " << product->name
                 << " quantity to " << item.quantity << endl;
            saveToFile();
            return true;
        }
    }

    // Item not already in the cart, so add as new
    items.push_back(CartItem(product, quantity));
    cout << "SUCCESS: Added " << quantity << " x " << product->name << " to cart" << endl;
    saveToFile();
    return true;
}

// Remove an entire product from the cart
bool ShoppingCart::removeItem(const string& productName) {
    string nameLower = productName;
    transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);

    for (auto it = items.begin(); it != items.end(); ++it) {
        string itemNameLower = it->product->name;
        transform(itemNameLower.begin(), itemNameLower.end(), itemNameLower.begin(), ::tolower);

        if (itemNameLower == nameLower) {
            cout << "SUCCESS: Removed " << it->product->name << " from cart" << endl;
            items.erase(it);
            saveToFile();
            return true;
        }
    }

    cout << "ERROR: Item not found in cart" << endl;
    return false;
}

// Display all cart items
void ShoppingCart::showCart() {
    if (items.empty()) {
        cout << "CART_EMPTY" << endl;
        return;
    }

    cout << "CART_START" << endl;
    for (const auto& item : items) {
        cout << item.product->name << "|"
             << item.quantity << "|"
             << item.product->price << "|"
             << (item.quantity * item.product->price) << endl;
    }
    cout << "CART_END" << endl;
    cout << "TOTAL: " << getTotal() << endl;    //final total amount print
}

double ShoppingCart::getTotal() {    //total price of cart
    double total = 0.0;
    for (const auto& item : items)
        total += item.product->price * item.quantity;
    return total;
}

// Checkout process (verify stock, deduct items, and clear cart)
void ShoppingCart::checkout(ProductManager& pm) {
    if (items.empty()) {
        cout << "ERROR: Cart is empty" << endl;
        return;
    }

    // Check stock availability before updating
    for (const auto& item : items) {
        Product* p = pm.getProduct(item.product->name);
        if (!p || p->stock < item.quantity) {
            cout << "ERROR: Insufficient stock for " << item.product->name << endl;
            return;
        }
    }

    // Deduct stock quantities
    for (const auto& item : items) {
        pm.updateStock(item.product->name, item.quantity);
    }

    
    pm.saveProductsToFile("products.txt");

    double total = getTotal();
    cout << "CHECKOUT_SUCCESS" << endl;
    cout << "TOTAL_PAID: " << total << endl;

    clear();    // Clear cart after payment
    saveToFile();  
}

void ShoppingCart::clear() {
    items.clear();
}

vector<CartItem> ShoppingCart::getItems() {
    return items;
}

// Load cart items from file
void ShoppingCart::saveToFile() {
    string filePath = getCartFilePath();
    ofstream file(filePath);
    if (!file.is_open()) {
        cerr << "ERROR: Could not open " << filePath << " for writing" << endl;
        return;
    }
        
    // Write each item as: name|qty
    for (auto& item : items) {
        file << item.product->name << "|" << item.quantity << "\n";
    }

    file.close();
}

void ShoppingCart::loadFromFile() {    // Load cart from file
    string filePath = getCartFilePath();
    ifstream file(filePath);
    if (!file.is_open()) {
        return;
    }

    items.clear();
    string line;
    while (getline(file, line)) {
        size_t delim = line.find('|');
        if (delim != string::npos) {
            string name = line.substr(0, delim);    //product name
            int qty = stoi(line.substr(delim + 1));    //quantity
            extern ProductManager productManager;
            Product* p = productManager.getProduct(name);     
            if (p)
                items.push_back(CartItem(p, qty));    //add to cart
        }
    }

    file.close();
}
