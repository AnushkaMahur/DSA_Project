
#include "cart.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <direct.h>   
using namespace std;


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

bool ShoppingCart::addItem(Product* product, int quantity) {
    if (!product || quantity <= 0)
        return false;

    if (product->stock < quantity) {
        cout << "ERROR: Insufficient stock for " << product->name << endl;
        return false;
    }

    for (auto& item : items) {
        if (item.product->name == product->name) {
            if (product->stock < item.quantity + quantity) {
                cout << "ERROR: Insufficient stock" << endl;
                return false;
            }
            item.quantity += quantity;
            cout << "SUCCESS: Updated " << product->name
                 << " quantity to " << item.quantity << endl;
            saveToFile();
            return true;
        }
    }

    items.push_back(CartItem(product, quantity));
    cout << "SUCCESS: Added " << quantity << " x " << product->name << " to cart" << endl;
    saveToFile();
    return true;
}

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
    cout << "TOTAL: " << getTotal() << endl;
}

double ShoppingCart::getTotal() {
    double total = 0.0;
    for (const auto& item : items)
        total += item.product->price * item.quantity;
    return total;
}

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

    clear();
    saveToFile();  
}

void ShoppingCart::clear() {
    items.clear();
}

vector<CartItem> ShoppingCart::getItems() {
    return items;
}


void ShoppingCart::saveToFile() {
    string filePath = getCartFilePath();
    ofstream file(filePath);
    if (!file.is_open()) {
        cerr << "ERROR: Could not open " << filePath << " for writing" << endl;
        return;
    }

    for (auto& item : items) {
        file << item.product->name << "|" << item.quantity << "\n";
    }

    file.close();
}

void ShoppingCart::loadFromFile() {
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
            string name = line.substr(0, delim);
            int qty = stoi(line.substr(delim + 1));
            extern ProductManager productManager;
            Product* p = productManager.getProduct(name);
            if (p)
                items.push_back(CartItem(p, qty));
        }
    }

    file.close();
}
