#include <iostream>
#include "../../src/backend_cpp/cart.h"
#include "../../src/backend_cpp/product.h"

using namespace std;

int main() {
    Cart cart;

    Product p1("101", "Laptop", 45000);
    Product p2("102", "Mouse", 500);

    cart.addProduct(p1);
    cart.addProduct(p2);

    if (cart.getTotalPrice() == 45500) {
        cout << "[PASS] Cart total calculation correct.\n";
    } else {
        cout << "[FAIL] Cart total calculation incorrect.\n";
    }

    cart.removeProduct("102");

    if (cart.getTotalPrice() == 45000) {
        cout << "[PASS] Cart remove function works.\n";
    } else {
        cout << "[FAIL] Cart remove function incorrect.\n";
    }

    return 0;
}
