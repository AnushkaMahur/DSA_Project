#ifndef CART_H
#define CART_H

#include "product.h"
#include <vector>
#include <string>

struct CartItem {
    Product* product;
    int quantity;

    CartItem(Product* p, int q) : product(p), quantity(q) {}
};

class ShoppingCart {
private:
    std::vector<CartItem> items;

public:
    bool addItem(Product* product, int quantity);
    bool removeItem(const std::string& productName);
    void showCart();
    double getTotal();
    void checkout(ProductManager& pm);
    void clear();
    std::vector<CartItem> getItems();

    void saveToFile();   // Save cart to file
    void loadFromFile(); // Load cart from file
};

#endif
