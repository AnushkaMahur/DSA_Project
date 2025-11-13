#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

struct Product {
    string id;
    string name;
    double price;
    int stock;
    string category;
    string brand;

    Product() : price(0.0), stock(0) {}
};

struct ProductFilters {
    double min_price = -1.0;
    double max_price = -1.0;
    vector<string> brands;
    string category = "";
};

// Insertion Sort
enum SortType {
    SORT_NONE,
    SORT_PRICE_ASC,      // Price (Low → High)
    SORT_PRICE_DESC,     // Price (High → Low)
    SORT_NAME_ASC,       // Name (A → Z)
    SORT_STOCK_DESC      // Stock (High → Low)
};

class ProductManager {
private:
    unordered_map<string, Product> products;

public:
    ProductManager() {}

    void loadProducts(const string& filename);
    void saveProductsToFile(const string& filename);

    Product* getProduct(const string& name);
    vector<Product> getAllProducts();
    vector<Product> getProductsByCategory(const string& category);
    bool updateStock(const string& name, int quantity);

    void displayProduct(const Product& p);

    vector<Product> applyFilters(const vector<Product>& input, const ProductFilters& f);

    vector<Product> sortProducts(vector<Product> input, SortType type);
};

#endif
