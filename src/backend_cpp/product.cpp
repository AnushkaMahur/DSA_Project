#include "product.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>
using namespace std;

static inline string trim(const string &s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

void ProductManager::loadProducts(const string& filename) {
    products.clear();
    ifstream in(filename);
    if (!in.is_open()) return;

    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        if (line[0] == '#') continue;

        vector<string> parts;
        string tmp;
        stringstream ss(line);
        while (getline(ss, tmp, '|')) parts.push_back(trim(tmp));
        if (parts.size() < 4) continue;

        Product p;
        p.name = parts[0];
        try { p.price = stod(parts[1]); } catch (...) { p.price = 0.0; }
        try { p.stock = stoi(parts[2]); } catch (...) { p.stock = 0; }
        p.category = (parts.size() > 3 ? parts[3] : "");
        p.rating = (parts.size() > 4 ? stod(parts[4]) : 0.0);
        p.brand = (parts.size() > 5 ? parts[5] : "");

        string key = p.name;
        transform(key.begin(), key.end(), key.begin(), ::tolower);
        products[key] = p;
    }
    in.close();
}

void ProductManager::saveProductsToFile(const string& filename) {
    ofstream out(filename);
    if (!out.is_open()) return;

    for (auto &pr : products) {
        const Product &p = pr.second;
        out << p.name << "|" << p.price << "|" << p.stock << "|" << p.category
            << "|" << p.rating << "|" << p.brand << "\n";
    }
    out.close();
}

Product* ProductManager::getProduct(const string& name) {
    string key = name;
    transform(key.begin(), key.end(), key.begin(), ::tolower);
    auto it = products.find(key);
    if (it == products.end()) return nullptr;
    return &it->second;
}

vector<Product> ProductManager::getAllProducts() {
    vector<Product> v;
    v.reserve(products.size());
    for (auto &pr : products) v.push_back(pr.second);
    return v;
}

vector<Product> ProductManager::getProductsByCategory(const string& category) {
    vector<Product> result;
    string catLower = category;
    transform(catLower.begin(), catLower.end(), catLower.begin(), ::tolower);

    for (auto &pr : products) {
        string pcat = pr.second.category;
        string pcatLower = pcat;
        transform(pcatLower.begin(), pcatLower.end(), pcatLower.begin(), ::tolower);
        if (pcatLower == catLower) result.push_back(pr.second);
    }
    return result;
}

bool ProductManager::updateStock(const string& name, int quantity) {
    string key = name;
    transform(key.begin(), key.end(), key.begin(), ::tolower);
    auto it = products.find(key);
    if (it == products.end()) return false;
    if (it->second.stock + quantity < 0) return false;
    it->second.stock += quantity;
    return true;
}

void ProductManager::displayProduct(const Product& p) {
    cout << p.name << "|" << p.price << "|" << p.stock << "|" << p.category
         << "|" << p.rating << "|" << p.brand << endl;
}

vector<Product> ProductManager::applyFilters(const vector<Product>& input, const ProductFilters& f) {
    vector<Product> out;
    out.reserve(input.size());

    for (const Product &p : input) {
        if (f.min_price >= 0.0 && p.price < f.min_price) continue;
        if (f.max_price >= 0.0 && p.price > f.max_price) continue;
        if (f.min_rating >= 0.0 && p.rating < f.min_rating) continue;

        if (!f.category.empty()) {
            string a = p.category;
            string b = f.category;
            transform(a.begin(), a.end(), a.begin(), ::tolower);
            transform(b.begin(), b.end(), b.begin(), ::tolower);
            if (a != b) continue;
        }

        if (!f.brands.empty()) {
            bool ok = false;
            string pbrand = p.brand;
            transform(pbrand.begin(), pbrand.end(), pbrand.begin(), ::tolower);

            for (const string &b : f.brands) {
                string lb = b;
                transform(lb.begin(), lb.end(), lb.begin(), ::tolower);
                if (lb == pbrand) { ok = true; break; }
            }
            if (!ok) continue;
        }

        out.push_back(p);
    }

    return out;
}
