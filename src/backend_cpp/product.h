#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

//all product info
struct Product {
    string id;
    string name;
    double price;
    int stock;
    string category;    //electronics,home,audio etc
    string brand;

    Product() : price(0.0), stock(0) {}
};

//filters applied during searching
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
    unordered_map<string, Product> products;    //stores all products using lowercase name as key

public:
    ProductManager() {}

    void loadProducts(const string& filename);    //read products from file and load them into the map
    void saveProductsToFile(const string& filename);

    Product* getProduct(const string& name);   
    vector<Product> getAllProducts();    //getting all the products
    vector<Product> getProductsByCategory(const string& category);    //getting products with same category
    bool updateStock(const string& name, int quantity);    //update stock quantity after each checkout

    void displayProduct(const Product& p); 

    vector<Product> applyFilters(const vector<Product>& input, const ProductFilters& f);    //apply filters

    vector<Product> sortProducts(vector<Product> input, SortType type);    //display product list acc to sort type
};

#endif
