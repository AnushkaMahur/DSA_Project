#include "product.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
using namespace std;

void ProductManager::loadProducts(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "ERROR: Cannot open " << filename << endl;
        return;
    }

    products.clear();

    string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#')
            continue;

        string name, priceStr, stockStr, category;

        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);
        size_t p3 = line.find('|', p2 + 1);

        if (p1 == string::npos || p2 == string::npos || p3 == string::npos) {
            cerr << "Invalid line format: " << line << endl;
            continue;
        }

        name = line.substr(0, p1);
        priceStr = line.substr(p1 + 1, p2 - p1 - 1);
        stockStr = line.substr(p2 + 1, p3 - p2 - 1);
        category = line.substr(p3 + 1);

        double price = 0.0;
        int stock = 0;

        try {
            price = stod(priceStr);
            stock = stoi(stockStr);
        } catch (...) {
            cerr << "Error parsing line: " << line << endl;
            continue;
        }

        string nameLower = name;
        transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);

        products[nameLower] = Product(name, name, price, stock, category);
    }

    file.close();
}

Product* ProductManager::getProduct(const string& name) {
    string nameLower = name;
    transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);

    auto it = products.find(nameLower);
    if (it != products.end()) {
        return &(it->second);
    }
    return nullptr;
}

vector<Product> ProductManager::getAllProducts() {
    vector<Product> result;
    for (auto& pair : products) {
        result.push_back(pair.second);
    }
    return result;
}

bool ProductManager::updateStock(const string& name, int quantity) {
    Product* p = getProduct(name);
    if (p && p->stock >= quantity) {
        p->stock -= quantity;
        return true;
    }
    return false;
}

void ProductManager::displayProduct(const Product& p) {
    cout << p.name << "|" << p.price << "|" << p.stock << "|" << p.category << endl;
}

void ProductManager::saveProductsToFile(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "ERROR: Could not open " << filename << " for saving updated stock" << endl;
        return;
    }

    for (const auto& pair : products) {
        const Product& p = pair.second;
        file << p.name << "|" << p.price << "|" << p.stock << "|" << p.category << "\n";
    }

    file.close();
}
vector<Product> ProductManager::getProductsByCategory(const string& category) {
    vector<Product> result;
    string categoryLower = category;
    transform(categoryLower.begin(), categoryLower.end(), categoryLower.begin(), ::tolower);

    for (auto& pair : products) {
        string catLower = pair.second.category;
        transform(catLower.begin(), catLower.end(), catLower.begin(), ::tolower);
        if (catLower == categoryLower) {
            result.push_back(pair.second);
        }
    }
    return result;
}
