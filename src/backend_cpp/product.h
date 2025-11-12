#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

// Represents a single product in the store
struct Product {
    string id;        // Optional unique ID
    string name;      // Product name
    double price;     // Product price
    int stock;        // Units in stock
    string category;  // Product category

    Product() : id(""), name(""), price(0.0), stock(0), category("") {}

    Product(string i, string n, double p, int s, string c)
        : id(i), name(n), price(p), stock(s), category(c) {}
};

class ProductManager {
private:
    unordered_map<string, Product> products;  

public:
    
    void loadProducts(const string& filename);

    void saveProductsToFile(const string& filename);

    Product* getProduct(const string& name);

    vector<Product> getAllProducts();

    vector<Product> getProductsByCategory(const string& category);

    bool updateStock(const string& name, int quantity);

    void displayProduct(const Product& p);
};

#endif
