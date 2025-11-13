#ifndef CART_H
#define CART_H

#include "product.h"
#include <vector>
#include <string>


struct CartItem {
    Product* product;    // Stores the product pointer + how many of that item
    int quantity;    // Represents one item inside the cart

    CartItem(Product* p, int q) : product(p), quantity(q) {}
};

//all cart operations (main class)
class ShoppingCart {
private:
    std::vector<CartItem> items;

public:
    bool addItem(Product* product, int quantity);
    bool removeItem(const std::string& productName);    //add or remove from the cart
    void showCart();    //view complete cart items
    double getTotal();    //total cost of cart
    void checkout(ProductManager& pm);
    void clear();    //clear cart
    std::vector<CartItem> getItems();

    void saveToFile();   // Save cart to file
    void loadFromFile(); // Load cart from file
};

#endif
